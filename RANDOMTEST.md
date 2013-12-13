I added Cymric as a generator to the NIST STS random testing suite.  It passes
all of these tests.  And on consecutive runs, the tests for which it gets the
lowest scores often end up popping back up to 100/100.  This is what you would
expect from analyzing the output of a strong stream cipher:

~~~
------------------------------------------------------------------------------
RESULTS FOR THE UNIFORMITY OF P-VALUES AND THE PROPORTION OF PASSING SEQUENCES
------------------------------------------------------------------------------
   generator is <cymric using ChaCha20>
------------------------------------------------------------------------------
 C1  C2  C3  C4  C5  C6  C7  C8  C9 C10  P-VALUE  PROPORTION  STATISTICAL TEST
------------------------------------------------------------------------------
 11  19   7   6   9   5   9  12  10  12  0.115387    100/100     Frequency
  6   8   8   6  11  14  11  14  12  10  0.554420     99/100     BlockFrequency
 10  13  12  13   8   6  10   6  10  12  0.719747     99/100     CumulativeSums
 12  10  11   8  12  10   7   9   9  12  0.971699     98/100     CumulativeSums
  6  11  10   9  12   4  11  11  17   9  0.275709    100/100     Runs
  9  17  13  11   4  10  15   8   6   7  0.090936    100/100     LongestRun
  7  11   9   6  14  11  11  11  13   7  0.699313     99/100     Rank
 13  10   7   9  13   7  11  11  13   6  0.699313     99/100     FFT
  8   6   7  13  19  11   8   8  11   9  0.162606     98/100     NonOverlappingTemplate
 13   9   7   5   7  13  12   9  11  14  0.494392     99/100     NonOverlappingTemplate
  9   7  13  10  12   9  12   8  10  10  0.955835     99/100     NonOverlappingTemplate
 12   8  12   6   9   8   9  12  13  11  0.851383     99/100     NonOverlappingTemplate
 11   7  19   9  10  10   9  15   5   5  0.051942     97/100     NonOverlappingTemplate
  8   5  10  19  11   8   8  12  13   6  0.096578    100/100     NonOverlappingTemplate
 10   9  10  11   9  12  11   3   7  18  0.162606    100/100     NonOverlappingTemplate
 19   4   8  11   5  11  10   9   8  15  0.037566     98/100     NonOverlappingTemplate
 12   4  10  10  13  12   8  12  13   6  0.474986     98/100     NonOverlappingTemplate
  8  10   8   8   6  11  11   9  14  15  0.616305     99/100     NonOverlappingTemplate
 12  15   7  13  12  10   5  10   7   9  0.474986     99/100     NonOverlappingTemplate
 13   6   8  12   4  18  10  12   9   8  0.115387     97/100     NonOverlappingTemplate
 13  10  10  11   4   5  11  12  16   8  0.236810    100/100     NonOverlappingTemplate
  8   7  14   9  10   9  11   5  14  13  0.514124    100/100     NonOverlappingTemplate
  7  12   7  16  11   7   8  11  11  10  0.595549    100/100     NonOverlappingTemplate
 10  14   8  13  14   7   9  11   9   5  0.514124     98/100     NonOverlappingTemplate
  7  15   8  10  14  10  10  15   4   7  0.191687    100/100     NonOverlappingTemplate
 12  13   7  12   8   9   8  12   6  13  0.699313     98/100     NonOverlappingTemplate
  6   5   7  20   8  10   9   9  16  10  0.023545    100/100     NonOverlappingTemplate
 11   7   8  10   7  17  10   9   7  14  0.366918     98/100     NonOverlappingTemplate
 12   9   9   8   6  10   6  14  10  16  0.401199     98/100     NonOverlappingTemplate
 10  14  11   7  10  10   9   8  11  10  0.955835    100/100     NonOverlappingTemplate
 15  12  10  10   9  10   8  10   9   7  0.883171     99/100     NonOverlappingTemplate
 10  10   8   9   7  15  11   9   7  14  0.678686     99/100     NonOverlappingTemplate
 14   5   8  11   9  11  14  14  10   4  0.236810    100/100     NonOverlappingTemplate
  5  10  10  12   9   8  10  10  15  11  0.739918     99/100     NonOverlappingTemplate
  6   8   9   8  12  13  17   8   9  10  0.419021    100/100     NonOverlappingTemplate
 11  13  11   9   6   8  11   5  10  16  0.401199     99/100     NonOverlappingTemplate
 10  15  11  12   6  15   5   7   7  12  0.224821     99/100     NonOverlappingTemplate
 10   5   6  12  18  11   7   8  12  11  0.171867     98/100     NonOverlappingTemplate
  9  11   9  12  13  13   9   9  11   4  0.699313     99/100     NonOverlappingTemplate
 18   7   8   6  11   8  12  12   8  10  0.275709     98/100     NonOverlappingTemplate
 11  10   6  14  14  13  16   7   4   5  0.058984     97/100     NonOverlappingTemplate
 13   6  11   8  12  11  11  13   7   8  0.759756    100/100     NonOverlappingTemplate
 11  15  10   7  18   5  13   6   7   8  0.062821     98/100     NonOverlappingTemplate
  7   8  13  12   6  12   7  13  10  12  0.657933    100/100     NonOverlappingTemplate
 11  11   9   8   8   7  14  14   9   9  0.798139     98/100     NonOverlappingTemplate
 10  14  14   9   9  18   4   9   8   5  0.058984     98/100     NonOverlappingTemplate
  9   9  13   2   7   8   6  16  19  11  0.008266     98/100     NonOverlappingTemplate
  8  13  10   9  10  10  17  12   2   9  0.153763    100/100     NonOverlappingTemplate
  7  11  15  10  11   8  13   9   8   8  0.759756     98/100     NonOverlappingTemplate
  8  13  10   8  16   8  12   9   7   9  0.616305    100/100     NonOverlappingTemplate
  8  12  14   9   6   8  10  13   6  14  0.474986    100/100     NonOverlappingTemplate
  9   6  17  14   7   9   8  10  11   9  0.366918     99/100     NonOverlappingTemplate
  5  12  11  10   5  15  19   7   7   9  0.035174     99/100     NonOverlappingTemplate
  9   5  10  11  11  15  12   7  10  10  0.678686    100/100     NonOverlappingTemplate
 10  11   7   6  15   8  10  15  10   8  0.494392     99/100     NonOverlappingTemplate
  9  12  12  11   5  11  19  10   6   5  0.071177     99/100     NonOverlappingTemplate
 13  10   5   6   5  11  10   9  17  14  0.115387     98/100     NonOverlappingTemplate
 13   5   9   8  16   9  14  12   5   9  0.202268    100/100     NonOverlappingTemplate
  6  11   9   7  14  11   8  11  13  10  0.759756    100/100     NonOverlappingTemplate
 14  12   5   2  16  12  10   9   8  12  0.071177    100/100     NonOverlappingTemplate
  7  12   8  11  11  13   7   7  12  12  0.798139    100/100     NonOverlappingTemplate
 11  17   9  11  10   8  10   9   8   7  0.637119     98/100     NonOverlappingTemplate
 13  15   8   7  12  11  12   7   9   6  0.514124     99/100     NonOverlappingTemplate
 10   6  11   9  14   9  11   9   8  13  0.834308    100/100     NonOverlappingTemplate
  8  10   9  13  12  10   8   7   9  14  0.851383    100/100     NonOverlappingTemplate
  6   9  11  18  11   5   7  11  15   7  0.085587     98/100     NonOverlappingTemplate
  9  11   7   7  11  10  12  18   6   9  0.304126     98/100     NonOverlappingTemplate
  7   4   8  16  14  10  14   8   9  10  0.202268     98/100     NonOverlappingTemplate
  7   4   8   8  14  10  13   9  17  10  0.171867    100/100     NonOverlappingTemplate
 11   9  16   8   8  13   4  10  10  11  0.419021    100/100     NonOverlappingTemplate
 17  12  12  13   8  10  10   9   7   2  0.108791     98/100     NonOverlappingTemplate
 10   5  11  11  11  11  16   7  11   7  0.494392     99/100     NonOverlappingTemplate
 12   6  12   8  10  10  14  11  13   4  0.437274     99/100     NonOverlappingTemplate
 10   9  14   8  11   6   7  10  17   8  0.350485    100/100     NonOverlappingTemplate
  9  13   9  13  13  18   6   6   6   7  0.090936     99/100     NonOverlappingTemplate
  9  13  10   8   5   9  15  13   9   9  0.574903     99/100     NonOverlappingTemplate
  6  15   7  15  16  10   7   6   9   9  0.129620     98/100     NonOverlappingTemplate
  9  14  12  12  11   8   8   5   9  12  0.699313     99/100     NonOverlappingTemplate
 12   8  13   7   7   7   8  15  15   8  0.334538     99/100     NonOverlappingTemplate
 11   8   7   5   7  13   9   9  14  17  0.191687     97/100     NonOverlappingTemplate
 11  11   8  10   7   7  10  14  12  10  0.883171     98/100     NonOverlappingTemplate
 14   7  12  10   8   7  10  10   9  13  0.816537    100/100     NonOverlappingTemplate
  9   5   7  13  20  10   8   8  11   9  0.080519     98/100     NonOverlappingTemplate
  8  13  10  13  15   4   8  16   7   6  0.096578    100/100     NonOverlappingTemplate
 10  12  10   7   9  11   8  12   9  12  0.971699     98/100     NonOverlappingTemplate
 15   6  13   7   8   6   7  12  14  12  0.262249     98/100     NonOverlappingTemplate
  9  11   8  15   9  12  15  10   4   7  0.304126     99/100     NonOverlappingTemplate
 12  14   6  10   9   8  12  10  10   9  0.867692    100/100     NonOverlappingTemplate
  6   8  12   8  11   9  13  12  15   6  0.494392    100/100     NonOverlappingTemplate
 12  13   7   9   8  14  10  10   6  11  0.739918     99/100     NonOverlappingTemplate
  8  14   8   9  15  10  10   8   8  10  0.759756     99/100     NonOverlappingTemplate
  7  17  15   6   8  15   4   9  14   5  0.014550    100/100     NonOverlappingTemplate
 12  10   9  11  14  11   4   9   8  12  0.657933     97/100     NonOverlappingTemplate
 14  14  16   4   8   9   8   8   7  12  0.162606     99/100     NonOverlappingTemplate
 12   3  14   7   8  11  15   8  11  11  0.249284     98/100     NonOverlappingTemplate
  9   8  11  10  10   9  15   9   9  10  0.946308    100/100     NonOverlappingTemplate
 15  12   5  16   7  10  11   9   7   8  0.249284    100/100     NonOverlappingTemplate
 10  14   7   7  10  12   6  11  10  13  0.699313     99/100     NonOverlappingTemplate
 14  11  12   4   8   6  12  12  10  11  0.474986     97/100     NonOverlappingTemplate
 10  14   8  18   6  10   8  13   6   7  0.129620    100/100     NonOverlappingTemplate
 10  11  13   5   6  10  13   7  11  14  0.474986    100/100     NonOverlappingTemplate
 13  11  12   9  11   4  11  16   8   5  0.224821     97/100     NonOverlappingTemplate
 11   9   7  11  10  17   8  12  11   4  0.304126    100/100     NonOverlappingTemplate
 12   4   9  10  14  16   7   9   9  10  0.319084    100/100     NonOverlappingTemplate
  9  11  12  10  12  10  12   7   9   8  0.971699     99/100     NonOverlappingTemplate
 16  13   9   8   8   6  10  12   8  10  0.554420     99/100     NonOverlappingTemplate
 14   9  11   9   8   6   8   7  13  15  0.474986     98/100     NonOverlappingTemplate
 12  11  11  11  10   9  14   6  10   6  0.779188     99/100     NonOverlappingTemplate
  6  13  10  10   8  11  10  16  10   6  0.514124    100/100     NonOverlappingTemplate
  9  10  15  11   7  11   5   9  15   8  0.419021    100/100     NonOverlappingTemplate
  7   8  16   8   8   7   7  13  10  16  0.213309     99/100     NonOverlappingTemplate
 18  10   7  10   7  11   7   8  13   9  0.304126    100/100     NonOverlappingTemplate
 12   6  13  12   8  11  14  10   6   8  0.595549     98/100     NonOverlappingTemplate
 14  10   9   7  14  14   8  10   9   5  0.455937     99/100     NonOverlappingTemplate
  6  10  11   8  11  11  12  11  10  10  0.971699     99/100     NonOverlappingTemplate
 10   9  11   8   6  16  12   9  12   7  0.574903    100/100     NonOverlappingTemplate
  8  10  10   9   8  19  15   9   6   6  0.096578     99/100     NonOverlappingTemplate
  8  19   7   9  13   7   8  14   9   6  0.090936     99/100     NonOverlappingTemplate
 12  16  13  11   9  11  11   7   2   8  0.162606     97/100     NonOverlappingTemplate
  9  10  10   9   9   9  11   9  12  12  0.997823     98/100     NonOverlappingTemplate
 11   9   9  12   9  11  13   9   8   9  0.983453    100/100     NonOverlappingTemplate
 17   5  10   9  11  11   9  15   4   9  0.122325     98/100     NonOverlappingTemplate
 12   6  13  10  10   8   4  18  11   8  0.129620     97/100     NonOverlappingTemplate
  5  13   7  10  10  14  13   8  13   7  0.437274    100/100     NonOverlappingTemplate
 11  17   6   7   8  10  15   5  13   8  0.115387    100/100     NonOverlappingTemplate
 11   6   9  13  15   4  10  14   9   9  0.304126     99/100     NonOverlappingTemplate
 11  11   8  14  13   7  14   8   8   6  0.534146     99/100     NonOverlappingTemplate
 13  12   9   6  12  13   5   7  10  13  0.474986    100/100     NonOverlappingTemplate
  9  10   8  12  13   9   6  11  11  11  0.924076     99/100     NonOverlappingTemplate
  5   8  11   7  17   9  10  13  11   9  0.350485    100/100     NonOverlappingTemplate
  6   9  13  11   9  13  12   9   9   9  0.883171     98/100     NonOverlappingTemplate
  5  10  13  13  13  10  12   8   5  11  0.474986     99/100     NonOverlappingTemplate
 10  11   8  13   4  11   9   9   9  16  0.437274     98/100     NonOverlappingTemplate
  8  11  10   9   7  11   5  13  13  13  0.657933     99/100     NonOverlappingTemplate
 18   9  10   8  11   8   7   8  14   7  0.262249     99/100     NonOverlappingTemplate
 12   9   8   7  15  14  10   7  11   7  0.554420     98/100     NonOverlappingTemplate
 10   7   7   9  10   9  12  12  14  10  0.883171     99/100     NonOverlappingTemplate
 14  16   8   9   6   7  13  11  10   6  0.289667     97/100     NonOverlappingTemplate
  8   7   8  12   4   8  17  14  11  11  0.171867     99/100     NonOverlappingTemplate
 11   7   9   9   4  13  11   9  15  12  0.455937    100/100     NonOverlappingTemplate
 12   8  10  11   4  12   9   8  15  11  0.534146     98/100     NonOverlappingTemplate
 10   4  11  11  11   7   9  20   8   9  0.080519     99/100     NonOverlappingTemplate
  9   9   8   6   8  18   9  16   9   8  0.153763     98/100     NonOverlappingTemplate
  8  12  13  14   7   7  10  10   8  11  0.779188     99/100     NonOverlappingTemplate
 10   8  11   7  14   8  12   7  14   9  0.699313    100/100     NonOverlappingTemplate
 10  10   7   9  11   9  13  12   9  10  0.978072     99/100     NonOverlappingTemplate
 16  11  12   8  15   4   8   5  13   8  0.096578     99/100     NonOverlappingTemplate
 12   7  11  15   6   6  14  14   9   6  0.213309     98/100     NonOverlappingTemplate
  9   7  14   8  14   6  11   5   9  17  0.129620     99/100     NonOverlappingTemplate
 10  13  10  10   9   8  12   9  13   6  0.883171    100/100     NonOverlappingTemplate
  6  14  11   3   9  10  10   9  13  15  0.224821    100/100     NonOverlappingTemplate
  7  13  11  14   7  18   7   8   6   9  0.129620     98/100     NonOverlappingTemplate
 10   7   9   4   7  13  14  16   8  12  0.191687     99/100     NonOverlappingTemplate
 14  11  16  10   9  12   4   8   8   8  0.304126     98/100     NonOverlappingTemplate
 13  10  14  12   9  10  10   7  11   4  0.574903     99/100     NonOverlappingTemplate
 14   7  12  10   8   8   9  10   9  13  0.851383    100/100     NonOverlappingTemplate
 11  10   9   9   8  11  11   4  14  13  0.637119     98/100     OverlappingTemplate
 13  11   5   8  19   8   9  10   6  11  0.115387    100/100     Universal
 12   8  14   7   9  10  12  13   9   6  0.699313     97/100     ApproximateEntropy
  5   5   6   8   7   7   8   3   6   8  0.900104     62/63      RandomExcursions
  6   3   7   6   5   5   8   5  10   8  0.723129     63/63      RandomExcursions
  7   5   3   7   4   7   8   5   9   8  0.756476     61/63      RandomExcursions
  5   7   8   7   3   6   7   7   6   7  0.957319     63/63      RandomExcursions
  7   9   3   8   7   8   6   5   4   6  0.788728     61/63      RandomExcursions
  7   7   4   9   8   7   4   8   3   6  0.723129     62/63      RandomExcursions
  6   6   4  10   7   7   3   9   7   4  0.585209     63/63      RandomExcursions
  7   6   8   6   8   7   8   3   6   4  0.875539     62/63      RandomExcursions
  3   9  11   6   5   5   7   7   6   4  0.484646     62/63      RandomExcursionsVariant
  4   8   7  11   5   4   5   5   9   5  0.484646     62/63      RandomExcursionsVariant
  4   6  11   9   6   3   8   8   5   3  0.287306     62/63      RandomExcursionsVariant
  5   6   7   6  10   9   5   7   2   6  0.585209     62/63      RandomExcursionsVariant
  6   6   6   4   8   3   8   7  10   5  0.689019     62/63      RandomExcursionsVariant
  5   6   1  12   5   1   5   6   9  13  0.003577     62/63      RandomExcursionsVariant
  6   4   9   4   7   2  11   6   7   7  0.337162     62/63      RandomExcursionsVariant
  5   6   9   6   5   7   7   3   9   6  0.819544     62/63      RandomExcursionsVariant
  6   4   6  11   6   5   4   5   7   9  0.585209     61/63      RandomExcursionsVariant
  8   7   6   6   7   6   6   5   8   4  0.980883     63/63      RandomExcursionsVariant
 12   5   5   9   5   4   3   6   8   6  0.287306     63/63      RandomExcursionsVariant
 12   7   4   6   7   5   4   6   4   8  0.422034     62/63      RandomExcursionsVariant
  8   8  10   8   2   5   6   3   7   6  0.422034     61/63      RandomExcursionsVariant
  7   6  11   7   4   5   7   8   5   3  0.551026     60/63      RandomExcursionsVariant
  7   7   8   8   7  12   2   2   5   5  0.141256     62/63      RandomExcursionsVariant
  7   9   7  10   7   6   6   2   7   2  0.337162     62/63      RandomExcursionsVariant
  8   7  11   4   8   6   5   6   4   4  0.551026     62/63      RandomExcursionsVariant
  7   5  11   9   5   5   6   5   3   7  0.517442     63/63      RandomExcursionsVariant
 15  14  11  10  14   8   4  11   9   4  0.137282     99/100     Serial
 13  13  11  11   9   9   5  12  12   5  0.534146     98/100     Serial
  7  13  11  15   9  10   9   8  10   8  0.798139    100/100     LinearComplexity


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
The minimum pass rate for each statistical test with the exception of the
random excursion (variant) test is approximately = 96 for a
sample size = 100 binary sequences.

The minimum pass rate for the random excursion (variant) test
is approximately = 60 for a sample size = 63 binary sequences.

For further guidelines construct a probability table using the MAPLE program
provided in the addendum section of the documentation.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
~~~



Here's the test code in case you want to recreate my results:

~~~
#include "cymric.h"
#include <assert.h>

void
ChaChaTest()
{
	int		i, num_0s, num_1s, bitsRead;
	int		done;
	
	if ( ((epsilon = (BitSequence *) calloc(tp.n,sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	assert(cymric_init() == 0);

	cymric_rng rng;
	assert(0 == cymric_seed(&rng, 0, 0));

	for ( i=0; i<tp.numOfBitStreams; i++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		do {
			char key[32];

			assert(0 == cymric_random(&rng, key, 32));

			done = convertToBits((BYTE*)key, 256, tp.n, &num_0s, &num_1s, &bitsRead);
		} while ( !done );

		fprintf(freqfp, "\t\tChaCha Stream Done : BITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);
}
~~~

Note that I am seeding once and generating 32 bytes at a time, which is actually
reseeding the ChaCha function and only ever using the first 32 bytes of output.
I was only slightly worried that this would lead to biases, but this demonstrates
that no bias exists as expected.

I had to add a directory for CYMRIC and add it to the various lists around the
source code.

