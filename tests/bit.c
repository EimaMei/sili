#define SI_IMPLEMENTATION 1
#include <sili.h>

#if SI_COMPILER_GCC || SI_COMPILER_CLANG
#define test(type, min, max) \
	do { \
		siTime ts = si_timeStampStart(); \
		for (i64 a = min; a < max - max / 16; a += max / 16) { \
			type limit = si_max(type, 1, max / UINT16_MAX / 64); \
			for (i64 b = min; b < max - limit; b += limit) { \
				type res[2]; \
				SI_ASSERT_FMT(si_checkAdd(type, (type)a, (type)b, &res[0]) == __builtin_add_overflow((type)a, (type)b, &res[1]), SI_STR(#type ": %lli %lli %lli %lli"), a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
				\
				SI_ASSERT_FMT(si_checkSub(type, (type)a, (type)b, &res[0]) == __builtin_sub_overflow((type)a, (type)b, &res[1]), SI_STR(#type ": %lli %lli %lli %lli"), a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
				\
				SI_ASSERT_FMT(si_checkMul(type, (type)a, (type)b, &res[0]) == __builtin_mul_overflow((type)a, (type)b, &res[1]), SI_STR(#type ": %lli %lli %lli %lli"), a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
			} \
		} \
		si_timeStampPrintSince(ts); \
	} while (0)
#else
	#define test(type, min, max)
#endif

#define bit__test(func, check, bit) { \
	u64 tests[] = {UINT64_MAX, SI_BIT(63), 1, 0xAAAAAAA, 0x22041}; \
	for_range (j, 0, countof(tests)) { \
		for_range (k, 0, 2) { \
			u64 test_in = tests[j]; \
			for_range (i, 0, 65) { \
				i32 result = func(u64, test_in); \
				i32 correct = check(test_in, bit); \
				if (result != correct) { \
					si_printfLn("%L: %S: %064#lb: result = %i, correct = %i (j = %zi, k = %zi)", SI_CALLER_LOC, SI_STR(#func), test_in, result, correct, j, k); \
					SI_PANIC(); \
				} \
				if (k == 0) { \
					test_in >>= 1; \
				} \
				else { \
					test_in <<= 1; \
				} \
			} \
		} \
	} \
}


siIntern
i32 bits_count(u64 n, u32 bit) {
    i32 count = 0;
    for_range (i, 0, 64) {
        count += (n & 1) == bit;
        n >>= 1;
    }
    return count;
}

siIntern
i32 bits_countLeading(u64 n, u32 bit) {
    i32 count = 0;
    for (i32 i = 63; i >= 0; i -= 1) {
        u32 current_bit = (n >> i) & 1;
        if (current_bit != bit) break;
        count += 1;
    }
    return count;
}

siIntern
i32 bits_countTrailing(u64 n, u32 bit) {
    i32 count = 0;
	for_range (i, 0, 64) {
        u32 current_bit = (n & SI_BIT(i)) != 0;
        if (current_bit != bit) break;
        count += 1;
    }
    return count;
}

siIntern
u64 bits_bitsRotateLeft(u64 n, u32 shift) {
    return (n << shift) | (n >> (64 - shift));
}

#define BIT_TEST(function) \
	bit__test(si_##function##Ones, bits_##function, 1) \
	bit__test(si_##function##Zeros, bits_##function, 0)

int main(void) {
	BIT_TEST(count)
	BIT_TEST(countLeading)
	BIT_TEST(countTrailing)

	{
		test(u8, 0, UINT8_MAX);
		test(u16, 0, UINT16_MAX);
		test(u32, 0, UINT32_MAX);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsign-conversion"
		test(u64, 0, UINT64_MAX);
		test(usize, 0, USIZE_MAX);
#pragma GCC diagnostic pop

		test(i8, INT8_MIN, INT8_MAX);
		test(i16, INT16_MIN, INT16_MAX);
		test(i32, INT32_MIN, INT32_MAX);
		test(i64, INT64_MIN, INT64_MAX);
		test(isize, ISIZE_MIN, ISIZE_MAX);
	}

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
}
