#include "cymric.h"

#include "Platform.hpp"
#include "Clock.hpp"
using namespace cat;

#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
using namespace std;

static Clock m_clock;

void print_buffer(const void *buffer, int bytes) {
	const u8 *b = (const u8 *)buffer;

	for (int ii = 0; ii < bytes; ++ii) {
		cout << hex << setfill('0') << setw(2) << (int)b[ii];
	}
}

/*
	This Quickselect routine is based on the algorithm described in
	"Numerical recipes in C", Second Edition,
	Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
	This code by Nicolas Devillard - 1998. Public domain.
*/
#define ELEM_SWAP(a,b) { register u32 t=(a);(a)=(b);(b)=t; }
static u32 quick_select(u32 arr[], int n)
{
	int low, high ;
	int median;
	int middle, ll, hh;
	low = 0 ; high = n-1 ; median = (low + high) / 2;
	for (;;) {
		if (high <= low) /* One element only */
			return arr[median] ;
		if (high == low + 1) { /* Two elements only */
			if (arr[low] > arr[high])
				ELEM_SWAP(arr[low], arr[high]) ;
			return arr[median] ;
		}
		/* Find median of low, middle and high items; swap into position low */
		middle = (low + high) / 2;
		if (arr[middle] > arr[high]) ELEM_SWAP(arr[middle], arr[high]) ;
		if (arr[low] > arr[high]) ELEM_SWAP(arr[low], arr[high]) ;
		if (arr[middle] > arr[low]) ELEM_SWAP(arr[middle], arr[low]) ;
		/* Swap low item (now in position middle) into position (low+1) */
		ELEM_SWAP(arr[middle], arr[low+1]) ;
		/* Nibble from each end towards middle, swapping items when stuck */
		ll = low + 1;
		hh = high;
		for (;;) {
			do ll++; while (arr[low] > arr[ll]) ;
			do hh--; while (arr[hh] > arr[low]) ;
			if (hh < ll)
				break;
			ELEM_SWAP(arr[ll], arr[hh]) ;
		}
		/* Swap middle item (in position low) back into correct position */
		ELEM_SWAP(arr[low], arr[hh]) ;
		/* Re-set active partition */
		if (hh <= median)
			low = ll;
		if (hh >= median)
			high = hh - 1;
	}
}
#undef ELEM_SWAP

void generate_key_test(int bytes) {
	cymric_rng R;

	char buffer[512];

	assert(sizeof(buffer) > bytes);

	vector<u32> ts, tg, td;
	double ws = 0, wg = 0, wd = 0;

	for (int iteration = 0; iteration < 10000; ++iteration) {
		double s0 = m_clock.usec();
		u32 t0 = Clock::cycles();

		assert(0 == cymric_seed(&R, buffer, bytes));

		u32 t1 = Clock::cycles();
		double s1 = m_clock.usec();

		ts.push_back(t1 - t0);
		ws += s1 - s0;

		s0 = m_clock.usec();
		t0 = Clock::cycles();

		assert(0 == cymric_random(&R, buffer, bytes));

		t1 = Clock::cycles();
		s1 = m_clock.usec();

		tg.push_back(t1 - t0);
		wg += s1 - s0;

		s0 = m_clock.usec();
		t0 = Clock::cycles();

		assert(0 == cymric_derive(&R, &R, buffer, bytes));

		t1 = Clock::cycles();
		s1 = m_clock.usec();

		td.push_back(t1 - t0);
		wd += s1 - s0;

		if (iteration < 5) {
			cout << "Generated random bytes: ";
			print_buffer(buffer, bytes);
			cout << endl;
		}

		// If the tests are taking very long (more than 10 seconds),
		if (ws + wg > 10000000.0) {
			cout << "Tests are taking a very long time.  This can be caused by slow /dev/random.  Stopping here at " << iteration << " iterations" << endl;
			break;
		}
	}

	u32 ms = quick_select(&ts[0], (int)ts.size());
	ws /= ts.size();
	u32 mg = quick_select(&tg[0], (int)tg.size());
	wg /= tg.size();
	u32 md = quick_select(&td[0], (int)td.size());
	wd /= td.size();

	double cpb = mg / (double)bytes;

	cout << "+ RNG init: `" << dec << ms << "` median cycles, `" << ws << "` avg usec" << endl;
	cout << "+ RNG generate: `" << dec << mg << "` median cycles, `" << cpb << "` cycles/byte, `" << wg << "` avg usec" << endl;
	cout << "+ RNG derive: `" << dec << md << "` median cycles, " << wd << "` avg usec" << endl;
}

int main() {
	cout << "Cymric unit tester" << endl;

	m_clock.OnInitialize();

	assert(0 == cymric_init());

	cout << "Generating 32 bytes:" << endl;
	generate_key_test(32);

	cout << "Generating 64 bytes:" << endl;
	generate_key_test(64);

	cout << "Generating 256 bytes:" << endl;
	generate_key_test(256);

	m_clock.OnFinalize();

	return 0;
}

