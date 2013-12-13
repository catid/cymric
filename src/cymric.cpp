/*
	Copyright (c) 2013 Christopher A. Taylor.  All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	* Neither the name of Cymric nor the names of its contributors may be
	  used to endorse or promote products derived from this software without
	  specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#include "cymric.h"
#include "blake2.h"
#include "chacha.h"
#include "Platform.hpp"
using namespace cat;


//// Default Flags

#define CYMRIC_DEV_RANDOM
#define CYMRIC_CYCLES
//#define CYMRIC_SRANDOM
#define CYMRIC_SRAND
#define CYMRIC_USEC
//#define CYMRIC_GETTID
#define CYMRIC_PTHREADS
#define CYMRIC_GETPID
#define CYMRIC_UNINIT
//#define CYMRIC_WINTID
//#define CYMRIC_WINCRYPT
//#define CYMRIC_WINMEM
//#define CYMRIC_ARC4RANDOM
#define CYMRIC_COUNTER


//// Operating System Detection

#if defined(CAT_OS_WINDOWS)
# undef CYMRIC_DEV_RANDOM
# define CYMRIC_WINCRYPT
# define CYMRIC_WINMEM
# undef CYMRIC_GETTID
# define CYMRIC_WINTID

#elif defined(CAT_OS_APPLE)

#elif defined(CAT_OS_SOLARIS)

#elif defined(CAT_OS_OS2)

#elif defined(CAT_OS_BSD)
# if defined __OpenBSD__
#  define CYMRIC_ARC4RANDOM
# endif

#elif defined(CAT_OS_LINUX)
# undef CYMRIC_PTHREADS
# define CYMRIC_GETTID

#elif defined(CAT_OS_XBOX)

#elif defined(CAT_OS_PS3)

#elif defined(CAT_OS_PS4)

#elif defined(CAT_OS_AIX)

#elif defined(CAT_OS_HPUX)

#elif defined(CAT_OS_IRIX)

#else

#endif


//// Entropy Source Imports

#include <stdlib.h>

#if defined(CYMRIC_CYCLES) || defined(CYMRIC_USEC)
#include "Clock.hpp"
#endif

#if defined(CYMRIC_DEV_RANDOM)
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef __OpenBSD__
// On OpenBSD, use /dev/srandom and also /dev/arandom through arc4random API
# define CYMRIC_RAND_FILE "/dev/srandom"
#else
# define CYMRIC_RAND_FILE "/dev/random"
#endif
# define CYMRIC_URAND_FILE "/dev/urandom"
#endif // CYMRIC_DEV_RANDOM

#if defined(CYMRIC_GETTID)

#ifdef CAT_OS_ANDROID
#include <unistd.h>
static u32 unix_gettid() {
	return (u32)gettid();
}

#elif defined(CAT_OS_LINUX)

#include <sys/types.h>
#include <linux/unistd.h>
#ifdef __NR_gettid
static u32 unix_gettid() {
	return (u32)syscall(__NR_gettid);
}
#define CYMRIC_HAS_GETTID
#endif // __NR_gettid

#endif // CAT_OS_LINUX

// If was not able to use gettid(),
#ifndef CYMRIC_HAS_GETTID
// Use pthreads instead
#undef CYMRIC_GETTID
#define CYMRIC_PTHREADS
#endif

#endif

#ifdef CYMRIC_WINCRYPT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#endif // CYMRIC_WINCRYPT

#ifdef CYMRIC_PTHREADS
#include <pthread.h>
#endif

#if defined(CYMRIC_SRAND) || defined(CYMRIC_SRANDOM)
#include <time.h>
#endif

#ifdef CYMRIC_COUNTER
#include "Atomic.hpp"
#endif


//// Support Routines

#ifdef CYMRIC_DEV_RANDOM

static bool read_file(const char *path, char *buffer, int bytes) {
	int remaining = bytes, retries = 100;

	do {
		int len = 0;

		// Attempt to read the file
		int random_fd = open(path, O_RDONLY);
		if (random_fd >= 0) {
			len = read(random_fd, buffer, remaining);
			close(random_fd);
		}

		// If read request failed,
		if (len <= 0) {
			if (--retries >= 0) {
				continue;
			} else {
				break;
			}
		}

		// Subtract off the read byte count from request size
		remaining -= len;
		buffer += len;
	} while (remaining > 0);

	return remaining <= 0;
}

#endif // CYMRIC_DEV_RANDOM

#ifdef CYMRIC_COUNTER

#ifdef CYMRIC_UNINIT
static u32 m_counter;
#else
static u32 m_counter = 0;
#endif

static u32 get_counter() {
	// Note this may not be thread-safe if the platform is unsupported
	return Atomic::Add(&m_counter, 1);
}

#endif // CYMRIC_COUNTER


#ifdef __cplusplus
extern "C" {
#endif

int _cymric_init(int expected_version) {
	return (expected_version == CYMRIC_VERSION) ? 0 : -1;
}


//// Generator Seed API

int cymric_seed(cymric_rng *R, const void *seed, int bytes) {
	// Self-test, verified to fail if set too high
	static const int MIN_ENTROPY = 425;
	int entropy = 0;

	// Initialize BLAKE2 state for 512-bit output
	blake2b_state state;

	// Mix in previous or uninitialized state
	if (blake2b_init_key(&state, 64, R->internal, 64)) {
		return -1;
	}

	// Mix in the seed
	if (seed && bytes > 0) {
		if (blake2b_update(&state, (const u8 *)seed, bytes)) {
			return -1;
		}
	}

#ifdef CYMRIC_USEC
	Clock clock;
	clock.OnInitialize();
#endif

#ifdef CYMRIC_CYCLES
	{
		// Mix in initial cycle count
		u32 t0 = Clock::cycles(false);
		if (blake2b_update(&state, (const u8 *)&t0, sizeof(t0))) {
			return -1;
		}
		CAT_SECURE_CLR(&t0, sizeof(t0));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_DEV_RANDOM
	{
		// Mix in /dev/random
		char rand_buffer[32];
		// This is a bit of a hack.  Most Linuxy systems "cache" about 20 bytes
		// for /dev/random.  Requesting 32 bytes may take up to a minute longer
		// and this would be really irritating for users of the library.  So
		// instead we grab just 20 bytes and then take the rest from urandom,
		// which is often implemented as a separate pool that will actually
		// provide additional entropy over /dev/random even when that device
		// is blocking waiting for more.
		if (!read_file(CYMRIC_RAND_FILE, rand_buffer, 20)) {
			return -1;
		}
		if (!read_file(CYMRIC_URAND_FILE, rand_buffer + 20, 12)) {
			return -1;
		}
		if (blake2b_update(&state, (const u8 *)rand_buffer, sizeof(rand_buffer))) {
			return -1;
		}
		CAT_SECURE_CLR(rand_buffer, sizeof(rand_buffer));
		entropy += 256;
	}
#endif

#ifdef CYMRIC_WINCRYPT
	{
		// Mix in Windows cryptographic RNG
		HCRYPTPROV hCryptProv;
		char win_buffer[32];
		if (!CryptAcquireContext(&hCryptProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT|CRYPT_SILENT)) {
			return -1;
		}
		if (hCryptProv && !CryptGenRandom(hCryptProv, sizeof(win_buffer), win_buffer)) {
			return -1;
		}
		if (hCryptProv && !CryptReleaseContext(hCryptProv, 0)) {
			return -1;
		}
		if (blake2b_update(&state, (const u8 *)win_buffer, sizeof(win_buffer))) {
			return -1;
		}
		CAT_SECURE_CLR(win_buffer, sizeof(win_buffer));
		entropy += 256;
	}
#endif

#ifdef CYMRIC_ARC4RANDOM
	{
		// Mix in arc4random
		char rand_buffer[32];
		arc4random_buf(rand_buffer, sizeof(rand_buffer));
		if (blake2b_update(&state, (const u8 *)rand_buffer, sizeof(rand_buffer))) {
			return -1;
		}
		CAT_SECURE_CLR(rand_buffer, sizeof(rand_buffer));
		entropy += 256;
	}
#endif
	
#ifdef CYMRIC_WINMEM
	{
		// Mix in Windows memory info
		MEMORYSTATUS mem_stats;
		GlobalMemoryStatus(&mem_stats);
		if (blake2b_update(&state, (const u8 *)&mem_stats, sizeof(mem_stats))) {
			return -1;
		}
		CAT_SECURE_CLR(&mem_stats, sizeof(mem_stats));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_GETPID
	{
		// Mix in process id
		u32 pid = (u32)getpid();
		if (blake2b_update(&state, (const u8 *)&pid, sizeof(pid))) {
			return -1;
		}
		CAT_SECURE_CLR(&pid, sizeof(pid));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_GETTID
	{
		// Mix in thread id
		u32 tid = unix_gettid();
		if (blake2b_update(&state, (const u8 *)&tid, sizeof(tid))) {
			return -1;
		}
		CAT_SECURE_CLR(&tid, sizeof(tid));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_PTHREADS
	{
		// Mix in pthread self id
		pthread_t myself = pthread_self();
		if (blake2b_update(&state, (const u8 *)&myself, sizeof(myself))) {
			return -1;
		}
		CAT_SECURE_CLR(&myself, sizeof(myself));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_WINTID
	{
		// Mix in thread id
		u32 gctid = GetCurrentThreadId();
		if (blake2b_update(&state, (const u8 *)&gctid, sizeof(gctid))) {
			return -1;
		}
		CAT_SECURE_CLR(&gctid, sizeof(gctid));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_USEC
	{
		// Mix in microsecond clock
		double s0 = clock.usec();
		if (blake2b_update(&state, (const u8 *)&s0, sizeof(s0))) {
			return -1;
		}
		CAT_SECURE_CLR(&s0, sizeof(s0));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_SRANDOM
	{
		// Mix in srandom
		srandom(random() ^ time(0));
		u32 sr[2] = { random(), random() };
		if (black2b_update(&state, (const u8 *)&sr[0], sizeof(sr))) {
			return -1;
		}
		CAT_SECURE_CLR(sr, sizeof(sr));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_SRAND
	{
		// Mix in srand
		srand(rand() ^ time(0));
		u32 r[2] = { rand(), rand() };
		if (blake2b_update(&state, (const u8 *)&r[0], sizeof(r))) {
			return -1;
		}
		CAT_SECURE_CLR(r, sizeof(r));
		entropy += 32;
	}
#endif

#ifdef CYMRIC_CYCLES
	{
		// Mix in final cycle count
		u32 t1 = Clock::cycles(false);
		if (blake2b_update(&state, (const u8 *)&t1, sizeof(t1))) {
			return -1;
		}
		CAT_SECURE_CLR(&t1, sizeof(t1));
		entropy += 9;
	}
#endif

#ifdef CYMRIC_COUNTER
	{
		// Mix in incrementing counter
		u32 c0 = get_counter();
		if (blake2b_update(&state, (const u8 *)&c0, sizeof(c0))) {
			return -1;
		}
		CAT_SECURE_CLR(&c0, sizeof(c0));
		entropy += 1;
	}
#endif

#ifdef CYMRIC_USEC
	clock.OnFinalize();
#endif

	// Squeeze out 512 random bits from entropy sources
	if (blake2b_final(&state, (u8 *)R->internal, 64)) {
		return -1;
	}

	// Erase BLAKE2 state
	CAT_SECURE_CLR(&state, sizeof(state));

	// Sanity check for compilation
	if (entropy < MIN_ENTROPY) {
		return -1;
	}

	return 0;
}


//// Random Number Generation API

int cymric_random(cymric_rng *R, char *buffer, int bytes) {
	static const int CHACHA_ROUNDS = 20;

	// Use low 32 bytes as a key for ChaCha20
	chacha_key *key = (chacha_key *)R->internal;

	// Use next 8 bytes as an IV for ChaCha20, reserving the top 24 bytes
	chacha_iv *iv = (chacha_iv *)(R->internal + 32);

	// Mix in some fast entropy sources
	u32 *iv_mix = (u32 *)iv;

#ifdef CYMRIC_CYCLES
	{
		// Mix in initial cycle count
		iv_mix[0] ^= Clock::cycles(false);
	}
#endif

#ifndef CYMRIC_UNINIT
	CAT_MEMCLR(buffer, bytes);
#endif

#ifdef CYMRIC_GETTID
	{
		// Mix in thread id
		iv_mix[1] ^= unix_gettid();
	}
#endif

#ifdef CYMRIC_PTHREADS
	{
		// Mix in pthread self id
		pthread_t myself = pthread_self();
		const u32 *words = (const u32 *)&myself;
		iv_mix[1] ^= words[0];
		if (sizeof(myself) > 4) {
			iv_mix[0] += words[1];
		}
	}
#endif

#ifdef CYMRIC_COUNTER
	{
		// Mix in incrementing counter
		const u32 c0 = get_counter();
		iv_mix[0] += CAT_ROL32(c0, 13);
	}
#endif

	// Note this is "encrypting" the uninitialized buffer with random
	// bytes from the ChaCha20 function.  Valgrind is not going to like
	// this but it is intentional.
	chacha(key, iv, (const u8 *)buffer, (u8 *)buffer, bytes, CHACHA_ROUNDS);

#ifdef CYMRIC_CYCLES
	{
		// Mix in final cycle count
		iv_mix[1] += Clock::cycles(false);
	}
#endif

	// Iterate the hash to erase the ChaCha20 key material
	if (blake2b((u8 *)R->internal, R->internal, 0, 64, 64, 0)) {
		return -1;
	}

	return 0;
}

#ifdef __cplusplus
}
#endif

