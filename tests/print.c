#define SI_IMPLEMENTATION 1
#include <sili.h>

#define TEST_PRINT(expectedStr, input, ...) \
	do { \
		char buffer[SI_KILO(4)]; \
		size_t expectedLen = si_cstrLen(expectedStr) + 1; \
		SI_ASSERT(expectedLen <= sizeof(buffer)); \
		\
		size_t len = si_snprintf(buffer, sizeof(buffer), input, __VA_ARGS__); \
		\
		if (expectedLen != len) { \
			si_printf( \
				"%CR" __FILE__ ":%i: Lengths are not the same:%C '%zu' vs '%zu'\n", \
				__LINE__, expectedLen, len, buffer \
			); \
		} \
		i32 res = strcmp(buffer, expectedStr); \
		SI_ASSERT_FMT(res == 0, "Wrong character at index %i: '%s'", res, buffer); \
		si_printf("Test at '" __FILE__ ":%i' has been completed.\n", __LINE__); \
	} while(0)

#define TEST_PRINT_REG(expectedStr) TEST_PRINT(expectedStr, expectedStr, 0)

int main(void) {
	/* From: https://en.cppreference.com/w/c/io/fprintf */
	const char* s = "Hello";
	TEST_PRINT_REG("Strings:\n");
	TEST_PRINT_REG(" padding:\n");
	TEST_PRINT("\t[Hello]\n", "\t[%s]\n", s);
	TEST_PRINT("\t[     Hello]\n", "\t[%10s]\n", s);
	TEST_PRINT("\t[Hello     ]\n", "\t[%-10s]\n", s);
	TEST_PRINT("\t[     Hello]\n", "\t[%*s]\n", 10, s);
	TEST_PRINT_REG(" truncating:\n");
	TEST_PRINT("\tHell\n", "\t%.4s\n", s);
	TEST_PRINT("\tHel\n", "\t%.*s\n", 3, s);

	TEST_PRINT("Characters:\tA %\n", "Characters:\t%c %%\n", 'A');

	TEST_PRINT_REG("Integers:\n");
	TEST_PRINT(
		"\tDecimal:\t1 2 000003 0  +4 -4\n",
		"\tDecimal:\t%i %d %.6i %i %.0i %+i %i\n",
					  1, 2,   3, 0,   0, +4, -4
	);

	TEST_PRINT("\tHexadecimal:\t5 a A 0x6\n", "\tHexadecimal:\t%x %x %X %#x\n", 5, 10, 10, 6);

#ifdef SI_NO_SILI_PRINTF_STYLE
	TEST_PRINT("\tOctal:\t\t12 012 04\n","\tOctal:\t\t%o %#o %#o\n", 10, 10, 4);
#else
	TEST_PRINT("\tOctal:\t\t12 0o12 0o4\n","\tOctal:\t\t%o %#o %#o\n", 10, 10, 4);
#endif

	TEST_PRINT_REG("Floating-point:\n");
	TEST_PRINT("\tRounding:\t1.500000 2 1.30000000000000004440892098500626\n", "\tRounding:\t%f %.0f %.32f\n", 1.5, 1.5, 1.3);
	TEST_PRINT("\tPadding:\t01.50 1.50  1.50\n", "\tPadding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
	TEST_PRINT("\tScientific:\t1.500000E+00 1.500000e+00\n", "\tScientific:\t%E %e\n", 1.5, 1.5);
	/* TEST_PRINT("\t0x1.8p+0 0X1.8P+0\n", "\tHexadecimal:\t%a %A\n", 1.5, 1.5); */
	TEST_PRINT("\tSpecial values:\t0/0=nan 1/0=inf\n", "\tSpecial values:\t0/0=%g 1/0=%g\n", 0.0 / 0.0, 1.0 / 0.0);

	TEST_PRINT_REG("Fixed-width types:\n");
	TEST_PRINT("\tLargest 32-bit value is 4294967295 or 0xffffffff\n", "\tLargest 32-bit value is %u or %#x\n", UINT32_MAX, UINT32_MAX);

	TEST_PRINT("true false 0b1 0b0\n", "%B %B %#b %#b\n", true, false, 1, 0);


	si_printf("%CYTest '" __FILE__ "' has been completed!%C\n");
}
