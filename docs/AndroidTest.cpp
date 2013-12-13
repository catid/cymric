#include "cymric-mobile/cymric.h"
#include "cymric-mobile/Clock.hpp"

bool testAndroid() {
	cat::Clock clock;
	clock.OnInitialize();

	cymric_rng R;
	if (cymric_init()) {
		return false;
	}

	double t0 = clock.usec();
	if (cymric_seed(&R, 0, 0)) {
		return false;
	}
	double t1 = clock.usec();
	char key[32];
	if (cymric_random(&R, key, 32)) {
		return false;
	}
	double t2 = clock.usec();

	clock.OnFinalize();

	__android_log_print(ANDROID_LOG_INFO, "CYMRIC", "{CYMRIC TEST} %f %f", (t1 - t0), (t2 - t1));

	return true;
}

