#define SI_IMPLEMENTATION 1
#include <sili.h>


i32 res[4];
i32 first[] = {10, 20, 30, 40};
i32 second[] =  {5, 5, 5, 5};

void performanceTest(void) {
	for_range (i, 0, countof(res)) {
		res[i] = first[i] + second[i];
	}
}

#if SI_ARCH_IS_X86
#include <immintrin.h>

void performanceTest2(void) {
	*(__m128i*)res = _mm_add_epi32(*(__m128i*)first, *(__m128i*)second);
}

#elif SI_ARCH_ARM64
#include <arm_neon.h>

void performanceTest2(void) {
	int32x4_t vec_first = vld1q_s32(first);
	int32x4_t vec_second = vld1q_s32(second);
	int32x4_t result = vaddq_s32(vec_first, vec_second);
	vst1q_s32(res, result);
}
#endif

int main(void) {
	{
		/* Checking if both functions correctly behave. */
		performanceTest();
		SI_ASSERT(res[0] == 15 && res[1] == 25 && res[2] == 35 && res[3] == 45);

		performanceTest2();
		SI_ASSERT(res[0] == 15 && res[1] == 25 && res[2] == 35 && res[3] == 45);
	}

	si_print("Running 'performanceTest()' 100000 times. Lets see how long it takes to execute that many times...\n");
	si_benchmarkRunsPerLoop(1000000, performanceTest());

	si_print("Now let's see how many times 'performanceTest()' can be executed in 5 seconds...\n");
	si_benchmarkExecutesPerMs(5000, performanceTest());

	si_print("The average performance:\n");
	si_benchmarkLoopsAvg(1000000, performanceTest());

	si_print("Now we will compare the performance stats between 'performanceTest()' and 'performanceTest2()':\n");
	si_benchmarkLoopsAvgCmp(100000, performanceTest(), performanceTest2());

	return 0;
}
