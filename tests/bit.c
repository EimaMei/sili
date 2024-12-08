#define SI_IMPLEMENTATION 1
#include <sili.h>

#if !SI_COMPILER_MSVC
#define test(type, min, max) \
	do { \
		u64 ts = si_timeStampStart(); \
		for (i64 a = min; a < max - max / 16; a += max / 16) { \
			type limit = si_max(type, 1, max / UINT16_MAX / 64); \
			for (i64 b = min; b < max - limit; b += limit) { \
				type res[2]; \
				SI_ASSERT_FMT(si_checkAdd(type, (type)a, (type)b, &res[0]) == __builtin_add_overflow((type)a, (type)b, &res[1]), #type ": %lli %lli %lli %lli", a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
				\
				SI_ASSERT_FMT(si_checkSub(type, (type)a, (type)b, &res[0]) == __builtin_sub_overflow((type)a, (type)b, &res[1]), #type ": %lli %lli %lli %lli", a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
				\
				SI_ASSERT_FMT(si_checkMul(type, (type)a, (type)b, &res[0]) == __builtin_mul_overflow((type)a, (type)b, &res[1]), #type ": %lli %lli %lli %lli", a, b, res[0], res[1]); \
				SI_ASSERT(res[0] == res[1]); \
			} \
		} \
		si_timeStampPrintSince(ts); \
	} while (0)
#else
	#define test(type, min, max)
#endif


int main(void) {
	{
		u64 value_in  = 1;
		u32 value_out = 63;
		b32 check;

		for_range (i, 0, 65) {
			u32 ans1 = si_numLeadingZeros(u64, value_in),
				ans2 = si_numLeadingOnes(u64, value_in);
			check = (value_in & SI_BIT(63)) != 0;
			si_printf("%064#lb: (%u, %u); (%u, %u)\n", value_in, ans1, value_out, ans2, check);
			SI_ASSERT(ans1 == value_out);
			SI_ASSERT(ans2 == check);

			value_in <<= 1;
			value_out -= 1;
			if (value_out == UINT32_MAX) { value_out = 64; }
		}
		si_print("\n");

		value_in  = UINT64_MAX;
		value_out = 64;
		check = 0;

		for_range (i, 0, 65) {
			u32 ans1 = si_numLeadingOnes(u64, value_in),
				ans2 = si_numLeadingZeros(u64, value_in);
			si_printf("%064#lb: (%u, %u); (%u, %u)\n", value_in, ans1, value_out, ans2, check);

			SI_ASSERT(ans1 == value_out);
			SI_ASSERT(ans2 == check);

			value_in <<= 1;
			value_out -= 1;
			if (value_out == 0) { check = 64; }
		}

	}
	if (0) {
		test(u8, 0, UINT8_MAX);
		test(u16, 0, UINT16_MAX);
		test(u32, 0, UINT32_MAX);
		test(u64, 0, UINT64_MAX);
		test(usize, 0, USIZE_MAX);

		test(i8, INT8_MIN, INT8_MAX);
		test(i16, INT16_MIN, INT16_MAX);
		test(i32, INT32_MIN, INT32_MAX);
		test(i64, INT64_MIN, INT64_MAX);
		test(isize, ISIZE_MIN, ISIZE_MAX);
	}

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
}
