#define SI_IMPLEMENTATION 1
#include <sili.h>


/* A regular function. */
void function_regular(void);
/* The SIMD version of the above function. */
void function_simd(void);


volatile i32 res[4];
volatile i32 first[4] = {10, 20, 30, 40};
volatile i32 second[4] =  {5, 5, 5, 5};


int main(void) {
	{
		/* Check if both functions return the correct output. */
		function_regular();
		SI_ASSERT(res[0] == 15 && res[1] == 25 && res[2] == 35 && res[3] == 45);

		function_simd();
		SI_ASSERT(res[0] == 15 && res[1] == 25 && res[2] == 35 && res[3] == 45);
	}
	si_print("Running 'function_regular()' 100000 times. Lets see how long it takes to execute that many times...\n");
	si_benchmarkRunsPerLoop(1000000, function_regular());

	si_print("Now let's see how many times 'function_regular()' can be executed in 5 seconds...\n");
	si_benchmarkExecutesPerMs(5000, function_regular());

	si_print("The average performance:\n");
	si_benchmarkLoopsAvg(1000000, function_regular());

	si_print("Now we will compare the performance stats between 'function_regular()' and 'function_simd()':\n");
	si_benchmarkLoopsAvgCmp(100000, function_regular(), function_simd());
}

void function_regular(void) {
	for_range (i, 0, countof(res)) {
		res[i] = first[i] + second[i];
	}
}

#if SI_ARCH_IS_X86

#include <emmintrin.h>

void function_simd(void) {
	*(__m128i*)res = _mm_add_epi32(*(__m128i*)first, *(__m128i*)second);
}

#elif SI_ARCH_ARM64

#include <arm_neon.h>

void function_simd(void) {
	int32x4_t vec_first = vld1q_s32(first);
	int32x4_t vec_second = vld1q_s32(second);
	int32x4_t result = vaddq_s32(vec_first, vec_second);
	vst1q_s32(res, result);
}

#elif SI_ARCH_IS_WASM

#include <wasm_simd128.h>

void function_simd(void) {
    v128_t vec_first = wasm_v128_load((rawptr)first);
    v128_t vec_second = wasm_v128_load((rawptr)second);
    v128_t vec_result = wasm_i32x4_add(vec_first, vec_second);
    wasm_v128_store((rawptr)res, vec_result);
}

#else
	#error "Other CPU platforms aren't supported"
#endif
