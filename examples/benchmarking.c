#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <immintrin.h>

i32 res[4];
static i32 first[] = {10, 20, 30, 40};
static i32 second[] =  {5, 5, 5, 5};

void performanceTest(void) {
	for_range (i, 0, countof(res)) {
		res[i] = first[i] + second[i];
	}
}

void performanceTest2(void) {
	__m128i result = _mm_add_epi32(*(__m128i*)first, *(__m128i*)second);
	memcpy(res, (i32*)&result, sizeof(__m128i));
}

int main(void) {
	si_print("Running 'performanceTest()' 30000 times. Lets see how long it takes to execute that many times...\n");
	si_benchmarkRunsPerLoop(30000, performanceTest());

	si_print("Now let's see how many times 'performanceTest()' can be executed in 5 seconds...\n");
	si_benchmarkExecutesPerMs(5000, performanceTest());

	si_print("The average performance:\n");
	si_benchmarkLoopsAvg(10000, performanceTest());

	si_print("Now we will compare the performance stats between 'performanceTest()' and 'performanceTest2()':\n");
	si_benchmarkLoopsAvgCmp(10000, performanceTest(), performanceTest2());

	return 0;
}
