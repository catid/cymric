#Cymric
### Portable secure random number generator

Designed to provide increments of 256 random bits for use in public key
cryptosystems as fast as possible on almost any platform.


#### Explicitly Supported Platforms

Cymric has been verified to work properly on the following platforms:

+ clang-3.5 build on 64-bit Linode VPS Debian
+ LLVM-clang3.3 build on 64-bit Apple iMac
+ LLVM-clang3.3 build on iPad 2
+ android-ndk-r9 gcc-androideabi-4.8 on Android 4.1.2 on Nexus S
+ MSVC2010 64-bit Windows 7 Core i7 laptop


#### Building

##### Mac/Linux

Run `make test` to verify that the generator works on your platform.

This also generates `libcymric.a`, which you should link to (-lcymric).  And
you should add the `include/cymric.h` file to your include path.


##### Windows

You can link to the 64-bit `bin/libcymric.lib` static library and include
`include/cymric.h` to use Cymric from an e.g. Visual Studio project.
There is an example test project under `msvc2010/` that demonstrates using
Cymric from a Visual Studio project.

The following instructions allow you to reproduce the `bin/libcymric.lib` binary:

Download LLVM from [http://llvm.org/builds/](http://llvm.org/builds/) for Windows to C:\LLVM\.
Download Mingw64 from [http://mingw-w64.sourceforge.net/](http://mingw-w64.sourceforge.net/) for Windows 64-bit to C:\mingw64\.

~~~
copy Makefile.mingw64 Makefile
c:\mingw64\bin\mingw32-make.exe release
~~~

This produces `libcymric.lib`, which can be linked to an MSVC2010 build.


##### iOS/Android

Run the included `./make-mobile.sh` which will generate the `cymric-mobile`
folder you can include in your iOS/Android project.

For Android, place the files under your project's jni folder and add the
following lines to your `jni/Android.mk` file:

~~~
# Include Cymric random number generator
CYMRIC_SRC_FILES = cymric-mobile/blake2b-ref.c				\
				   cymric-mobile/chacha_blocks_ref.c		\
				   cymric-mobile/chacha.c					\
				   cymric-mobile/Clock.cpp					\
				   cymric-mobile/cymric.cpp

LOCAL_SRC_FILES += $(CYMRIC_SRC_FILES)
~~~


#### Usage

Reading the header file `include/cymric.h` is a good idea.  It consists of
a simple API:

+ cymric_init() : Verify API compatibility
+ cymric_seed(cymric_rng *R, const void *seed = 0, int seed_len = 0) : Seed the generator
+ cymric_random(cymric_rng *R, void *buffer, int length_in_bytes) : Generate random numbers

##### Setup

On startup your application should call cymric_init() once and verify that it
returns 0 to check that the library is linked properly:

~~~
	if (cymric_init()) {
		throw "cymric: ABI compatibility failure";
	}
~~~

##### Generating private keys

Whenever you want to generate a new (e.g. 256-bit) secret key, run code as in
this example:

~~~
	char secret_key[32];

	cymric_rng rng;
	if (cymric_seed(&rng, 0, 0)) {
		// Handle failure
		return false;
	}
	if (cymric_random(&rng, secret_key, 32)) {
		// Handle failure
		return false;
	}
~~~

This will seed the generator with at least 256 bits of entropy and then generate
a 256-bit key.

In some cases the `cymric_seed` function may take 30 seconds to complete, based
on the availability of high-quality randomness in the operating system.  This
is most likely to be an issue on Linux servers, though in normal operation it
rarely happens.  It can happen that two applications are both requesting random
data at once, and one of them will block until the OS can service it.

To avoid blocking, you should call `cymric_seed` no more than once per minute.
In this case blocking should be very rare.

When used for generating private keys in a public key/private key cryptosystem,
it is recommended that `cymric_seed` is called for *each* new private key that is
generated.  Though if multiple keys need to be generated quickly it is still
safe to seed just once and generate them all at one time.

##### Generating IVs or nonces

When used for generating IVs or nonces, it is recommended to re-use the
`cymric_rng` object from the previous seeding and to only call
`cymric_random` as in the following example:

~~~
	char server_nonce[32];

	if (cymric_random(&rng, server_nonce, 32)) {
		// Handle failure
		return false;
	}
~~~

Take care to verify that the buffer length parameter matches the actual buffer
size in bytes, and that the return value is checked.  Do not use `assert()` to
check for failure since code inside `assert()` may not be compiled.

It is also safe and efficient to generate two or more random numbers at once:

~~~
	char random_bytes[96];
	char *server_nonce = random_bytes;
	char *message_iv = random_bytes + 32;
	char *server_key = random_bytes + 64;

	if (cymric_random(&rng, random_bytes, 96)) {
		// Handle failure
		return false;
	}
~~~

In the above case the random number generation will run about 3x faster.

When you are done with the `cymric_rng` object, no additional cleanup steps
are needed because the state that generated the last random number is securely
erased from memory after each output.

##### Reseeding

It may be a good idea to periodically reseed the generator.  For example, if
your server uses ephemeral public/private key pairs, then each new key pair
should be generated after reseeding.

The difficulty in this case is that `cymric_seed` can sometimes take a long
time to complete.  To avoid blocking new connections to your server, this
function should be run in a separate thread.  This can lead to a vulnerability
because the `cymric_seed` and `cymric_random` functions are not thread-safe.

The danger is that while `cymric_seed` is reseeding, `cymric_random` may be
updating the state too, which would lose the new seed.

The solution to this is to copy the `cymric_rng` object and run `cymric_seed`
in another thread on the copy instead of the main object.  When the thread
completes, use the copy to generate a new key, and either overwrite the main
object with the copy, or discard it.  Overwriting the main object is better,
since it will incorporate new entropy.  Discarding it is simpler and also safe.

If these steps are not taken, the generator will still work as normal, though
occasionally some of the seeds will get lost, which is not a complete disaster.
In other words, Cymric fails gracefully when used incorrectly.


#### Benchmarks

Secure random number generation is essential to any cryptosystem, and it is
also unexpectedly slow.  Cymric attempts to provide the best possible user-land
implementation while also running exceptionally fast.  It does not require any
background threads to operate, and it builds on top of existing fast primitives
to avoid becoming a bottleneck in realistic use cases.


##### cymric-mobile on iPad 2:

It takes roughly `1.06 ms` to seed, and `140 usec` to generate a 256-bit secret.


##### cymric-mobile on Nexus S:

It takes roughly `268 usec` to seed, and `29 usec` to generate a 256-bit secret.


##### libcymric.a on Macbook Air (1.7 GHz Core i5-2557M Sandy Bridge, July 2011):

`make test` results (Edited: Cycles with Turbo Boost off, and walltime with Turbo Boost on):

Generating 32 bytes:

~~~
Generated random bytes: 9a8df8b30de7e95007fe2ab126debc5ef6112e05d92266e7baada95c5536ab84
Generated random bytes: 4d3004e2af4817b86263d033b30870bc9dccbfe62b93b8d85787d3cdd49f0ad0
Generated random bytes: 556c30bacc17becd61313b0b41559143bccac222c2747fcb0023b81be505e0c5
Generated random bytes: 67958bce483d2855179a7c22bcff48271137ed3c93e3116cdbfabb42bb360891
Generated random bytes: aaf85821f14f936edcb589333c619dd60831a9e47db1b8c8db02ca6161a435b7
~~~

+ RNG init: `48636` median cycles, `17.8821` avg usec
+ RNG generate: `1816` median cycles, `56.75` cycles/byte, `0.7776` avg usec

Generating 64 bytes:

~~~
Generated random bytes: c7b7b92716d57dd9af5819161f9e18aba7bfb74f3acbe8d2bf3b9f69ac514e82ff390819d4d930213f39856bbed229ea6d11122838690578560eb9465597817f
Generated random bytes: fed373d5e68d6c5bb2da604229b2d60ed67417a4d9c969635fb7f4083edc03847299d49076cb762610d0958dd6bee40ef0f6beaae8fd13a1574389eb0e281d3e
Generated random bytes: 2b2b8d202a773acbb118db0e3763be0a1d7780c48aecde700aa31c41b65e4ef14d68cab29df90f9f5d2a99b42520467a48228056704e6570ba46a1ec9febf7f5
Generated random bytes: e92e086adf6c68d25f95ee20f4961118f34b7e5b2fa7fc20e99edc33902035cf5c909f33e05fb398509978f9b6060ae5db25386c41c283a6f7b28bb9b259fdf4
Generated random bytes: 3d82de4b3552dfc0ed5ae75d131bd2e41fd1ff4816da03077fe35a3c9520b7f4f7567889e9ded87b92b34e258ccdfa7d8f69ba40bbce107d1f4f8c02767918c4
~~~

+ RNG init: `48660` median cycles, `17.7798` avg usec
+ RNG generate: `1696` median cycles, `26.5` cycles/byte, `0.7217` avg usec

Generating 256 bytes:

~~~
Generated random bytes: 92e05c545043c611efd3b72331d0570d0d6869337ffe52a6338399d645ad77eca26c09ae3bcb743e8b5ee7959a45185c8ed0bd77ab7dcebc94abe15f6dc182a9e82b27f231fa1eda5cbd8005bb4f97f41b5506d966ba90c63116dcb47381cb630b34f62cc10b31a4d2c2fee3f700d535b92e6a266e2caf593458204c6b7dcba2747b0c2d1bb79cb742e4e71b8238d46ddf48511993859595254db2621e860d1505ee2b232fca803bcaae9e1a40e85df7914f10c1c6f28d2e594591df7a738393c754212048cef54ed9d09211c7cac1dd6e8d1eea88d57a1c97926f69dac966b294efd5915f3c8f69d88f6a5e49eee9979e8569cc0ded8fbbf9b825181dad7e9c
Generated random bytes: 77a15c9748fe62d39bc7a9b3dcff1bd08d0ef357ca888b85d1eabce92519ab24e640ba84a96e0723c5eb72b1f29299715ba518582b4947583f90f00500d0b70a58d773a03dcf141a662249f118d0a4380173b0f7862d4f1acdd1a25616afb6a449e2280384f479fcbf91c0f7c902b5c3e378031ca102f8abc1111e50cf20c5c68991930d86f9de52bdd766f0b8c31dc0bfab90c2b7cbfb6cc97a41ba6853628765cae75dbaaae9ad11e3589fc5d6684bde77b1be3bfa9f67c9031a40f3f7c84a87d8747947bd8f50bc225f7f8ac03fdbd201361989260063fed07dec648e3d178ecdb691bd8b0b25073c273d02894d818fb4ad664a0b88a20a7386c79b06236d
Generated random bytes: f6035e8624d64f09ad14cd657e48e93f490980d4596816a183f6b5d41be24752a82d530bce093fb05832c2d5255820cb0d1f3304346f619c0f94038bdbfab3fd6b74aeef1361cd03979e5b1bac274a658ebe1c1a10d81eed4391a9c3c30be0d7961810cad8a788bdc513da97e826ef3a88c2da68801b92de21c04c93e18b31a0d1d34942936827e21d0f9d64d537c20a85dc5e11da4cd4e1e555003d1be23a28ce53fc6653b5362f8df0920143f1aa60e46949a79f435906157ef0bb53a940782a5db4556f5f1ff9c3455aae2aa8c783f08730c9dd9037da69ca1c56432d26d3f9c80048dda281eb1914a3304e63980fb8b0fde672a1f7c01539924737e0460d
Generated random bytes: 93a19efa2b5c8496be7f68e7c87490ceb7e95a1c8adf3b82fb1e085dd526cc59ef238355ebad75f3e5a875942b2d7a819159adc08143711f3790f1a1fd3d00f4a2286578a143245e5c185d4232213db2b15c5e6f4a32e80eb817d68856c112f46cbd691c392f5aafecc75d8475a9f28277ba3a7cc2430c62f1b16be2c197afc4c892947cfa36abf086d7555af0071560d9248c13b9b1caea0973343730cda9651cbd9bb25218781e66afdfc2e02de1f4eec1ccd7685530a50b3e989f1e670e5e066ae8ec5c5b92ff99b589a37e8d17481a16d08b4ff0c3a7388b8b462af91337261f694e30d4f66714662754036a315942a8f3f2e60c49f0077eda537e89c764
Generated random bytes: 13aac4cebf6ba5fac84a368950972c422215350ac63c487a36558a7623be822dfc1f384a0a5842535b85348d995a711ec2a4a73e1c3452187d4bd948310d54232601414b6d4382d9017f437308ab5f57056c3a711bdb47be493b06557f4595bbe506ad5ee609c6d2a9cf1d2e0488f19e97478f3d0a747ae1ccf9958cdec7ff1fcb7aaef732b1f0211b91376acac8b95af8dac5c1885810adb787797c03400950bab1231e335bc0105c2072cb306bedc94eb088672aac8880c601bdec0ec6f4bee88bce069816462e32d02b2a1c243c1dbee0f03f38bdf4e76ad39b6af75455309a57a2a8d3b29a1453adc5407f1aca1294aa8db7c8c6925a4059287fbe6c80bd
~~~

+ RNG init: `49336` median cycles, `18.8097` avg usec
+ RNG generate: `2316` median cycles, `9.04688` cycles/byte, `1.0095` avg usec


##### Benchmark Discussion

You can compare this implementation with other alternatives by disabling Turbo
Boost on a Sandy Bridge computer and comparing cycle counts with the ones above.
It was unexpected that generating 64 bytes of output is faster than 32 bytes.

For comparison, the [Snowshoe](https://github.com/catid/snowshoe) elliptic curve
math library, which is 8x faster than OpenSSL on the Macbook Air used above, can
compute an EC-DH shared secret key in `136,840` cycles (with TB off) and
`52 usec` (with TB on).  And Cymric with full optimization takes `19 usec` to
generate a secret key.

It is eye-opening that a *fast* library like Cymric still adds 36% overhead to
the client side of an EC-DH key agreement protocol.  This means that random
number generation can actually be a bottleneck if it is not implemented in an
efficient manner, and so random number generation should be optimized just like
elliptic curve math.

In reality it is particularly important to optimize Cymric for servers since
this is where cryptography is a bottleneck in handling many connections per
second.  For these platforms, the optimized BLAKE2 and ChaCha primitives should
be used rather than the portable reference versions.  Furthermore, servers are
not usually generating new keys for each connection but rather only generating
nonces.  Cymric is particularly fast for generating nonces, taking under 1
microsecond for each `cymric_random` call: Less than 2% overhead.

For comparison, accessing the `/dev/urandom` device directly takes roughly
4 microseconds (4x slower) and does not offer the extra security properties
of Cymric, while also being less portable.


#### Details

##### Philosophy

The generator is designed to be used for key agreement clients and servers,
which are initialized once and then expected to generate secret keys and
nonces.  The runtime is expected to be "reasonable" without requiring
background threads or a lot of overhead to avoid users turning it off.

About 256 bits of entropy should be maintained.

Any data used to generate a random number should be cleared from memory shortly
after use to avoid it being discovered later, written so that it will not be
optimized away by the compiler.

The internal state of the generator should never be directly exposed in the output.

A certain amount of the internal state should be held in reserve and not used to
produce output directly, to make the next iteration of the state more difficult
to guess.

A small amount of additional entropy should be mixed in over time without affecting
the speed of the generator nor requiring a background thread.  I am assuming that
if someone is able to read the internal state of the generator then all bets are off,
so extra Fortuna-like staggered pools are overkill.

The BLAKE2 hash function is used to mix entropy sources together into key material,
which is faster than MD5 and roughly as secure as other SHA-3 candidates.

The ChaCha20 cipher is used to generate random bytes from parts of the key material,
which is roughly twice as fast as BLAKE2 to generate 64 byte buffers, and much faster
for larger output.

The generator is not expected to be thread-safe though it should have some resilience
to misuse by mixing in the thread ID and uninitialized data.


##### Entropy Sources

The following entropy sources are mixed together using the BLAKE2 function:

All:

+ Uninitialized memory from input state object to be filled (512 bits)
+ Two samples of the processor cycle counter (64 bits)
+ Microsecond-resolution timestamp (64 bits)
+ Atomically incrementing counter for each seed/generate call (32 bits)

Windows:

+ CryptoAPI: CryptGenRandom (256 bits)
+ srand() and rand() from stdlib (64 bits)
+ GetCurrentThreadId() (32 bits)

Linux/Android:

+ /dev/random (160 bits)
+ /dev/urandom (96 bits)
+ srand() and rand() from stdlib (64 bits)
+ gettid() or pthread_self() (32 bits)
+ getpid() (32 bits)

Linux-type systems are a little strange in that /dev/random only keeps about
20 bytes of entropy in reserve.  And it takes roughly 20 seconds for a VPS to
collect 8 bytes of entropy, which means asking for 32 bytes of entropy takes an
unreasonably long time.  By asking for only 20 bytes, most of the requests by
this library will complete immediately.  And the remaining 12 bytes are
satisfied by /dev/urandom, which is often implemented as a separate pool that
has a lot of entropy in reserve.  The other random sources help make up the
difference since /dev/urandom is a little questionable.

BSD:

+ /dev/random (160 bits)
+ /dev/urandom (96 bits)
+ srandom() and random() from stdlib (64 bits)
+ gettid() (32 bits)
+ getpid() (32 bits)

Apple/iOS:

+ /dev/random (160 bits)
+ /dev/urandom (96 bits)
+ srandom() and random() from stdlib (64 bits)
+ getpid() (32 bits)
+ pthread_self() (64 bits)

Apple OS X /dev/random does not actually block, so it is "iffy."

Some resilience to forking problems is incorporated into the generator, as it mixes
in uninitialized data and the thread identifer.  Adding in other sources further
mitigates any unknown problems with the operating-system entropy sources.


##### Random Number Generation

The ChaCha20 cipher is keyed with the low 32 bytes of the current internal state of
the Cymric generator and its IV is set to the next 8 bytes, which leaves the high
24 bytes held in reserve and unexposed through generated data.

XChaCha20 was considered and dropped since it is much slower.

The keyed cipher encrypts the uninitialized output buffer to generate random bytes,
which separates the generated data from the internal state of the generator, and is
also considerably faster than iterating a full hash function.

Thread ID, clock cycle, and atomically incrementing counters are collected during
generation for extra entropy mixing.  These are mixed into the state and used for
generating the current random number as well as updating the state.

After the random number is generated, the internal state is iterated by keying the
the BLAKE2 hash function with the 64 bytes of old state to produce the new 64 byte
state.


#### Credits

This software was written entirely by myself ( Christopher A. Taylor <mrcatid@gmail.com> ).  If you
find it useful and would like to buy me a coffee, consider [tipping](https://www.gittip.com/catid/).

