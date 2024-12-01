#define SI_IMPLEMENTATION 1
#include <sili.h>


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



int main(void) {
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

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColorAnsi_Yellow, true, false));
}
