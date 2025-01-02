#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <math.h>


#define TEST_PRINT(expectedStr, input, ...) \
	do { \
		u8 buffer[SI_KILO(8)]; \
		isize expectedLen = si_cstrLen(expectedStr); \
		SI_ASSERT(expectedLen <= si_sizeof(buffer)); \
		\
		isize len = si_bprintf(buffer, si_sizeof(buffer), input, __VA_ARGS__).len; \
		\
		if (expectedLen != len) { \
			si_printf( \
				"%C" __FILE__ ":%i:%C Lengths are not the same: '%zu' vs '%zu'\n", \
				si_printColor3bitEx(siPrintColor3bit_Red, true, false), \
				__LINE__, expectedLen, len, buffer \
			); \
		} \
		i32 res = si_memcompare(buffer, expectedStr, len); \
		SI_ASSERT_FMT(res == 0, "Wrong character at index %i: '%s'", res, buffer); \
		si_printf("Test at '" __FILE__ ":%i' has been completed.\n", __LINE__); \
	} while(0)

#define TEST_PRINT_REG(expectedStr) TEST_PRINT(expectedStr, expectedStr, 0)


int main(void) {
	/* From: https://en.cppreference.com/w/c/io/fprintf */
	const char* str = "Hello";
	TEST_PRINT_REG("Strings:\n");
	TEST_PRINT_REG(" padding:\n");
	TEST_PRINT("\t[Hello]\n", "\t[%s]\n", str);
	TEST_PRINT("\t[     Hello]\n", "\t[%10s]\n", str);
	TEST_PRINT("\t[Hello     ]\n", "\t[%-10s]\n", str);
	TEST_PRINT("\t[     Hello]\n", "\t[%*s]\n", 10, str);
	TEST_PRINT_REG(" truncating:\n");
	TEST_PRINT("\tHell\n", "\t%.4s\n", str);
	TEST_PRINT("\tHel\n", "\t%.*s\n", 3, str);

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
	{
		const f64 nan = NAN;
		const f64 inf = INFINITY;
		const f64 n_inf = -INFINITY;
		TEST_PRINT("\tSpecial values:\t0/0=nan 1/0=inf -1/0=-inf\n", "\tSpecial values:\t0/0=%g 1/0=%g -1/0=%g\n", nan, inf, n_inf);
	}

	TEST_PRINT_REG("Fixed-width types:\n");
	TEST_PRINT("\tLargest 32-bit value is 4294967295 or 0xffffffff\n", "\tLargest 32-bit value is %u or %#x\n", UINT32_MAX, UINT32_MAX);

#ifndef SI_NO_SILI_PRINTF_STYLE
	TEST_PRINT("true false 0b1 0b0\n", "%B %B %#b %#b\n", true, false, 1, 0);
	TEST_PRINT("qwertyuiop\n", "%S\n", SI_STR("qwertyuiop"));
#endif

	si_print("================\nPrint colour tests:\nANSI/3-bit colour:\n");
	for_range (id, siPrintColor3bit_Black, (siPrintColor3bit)(siPrintColor3bit_White + 1)) {
		siPrintColor clr = si_printColor3bit(id),
					 bold = si_printColor3bitEx(id, true, false),
					 light = si_printColor3bitEx(id, false, true),
					 both = si_printColor3bitEx(id, true, true);
		si_printf("\t%CColor %i:%C %CBold%C %CLight%C %CAll%C\n", clr, id, bold, light, both);
	}

	si_print("\n8-bit colour:\n\t");
	for_range (i, 0, UINT8_MAX + 1) {
		siPrintColor clr = si_printColor8bit((u8)i);
		si_printf("%C% 3i%C ", clr, i);

        if (i == 15 || (i > 15 && (i - 15) % 6 == 0)) {
            si_printf("\n\t");
        }
	}
	si_printf("\n24-bit colour (%B):\n\t", si_printHas24bitColor());

    for_range (column, 0, 77) {
        i32 r = 255 - (column * 255 / 76);
        i32 g = (column * 510 / 76);
        i32 b = (column * 255 / 76);

        if (g > 255) {
            g = 510 - g;
        }

		/* TODO(EimaMei): On Windows a newline appears after each print for whatever reason. */
		si_printf("%C0%C", si_printColor24bit((u8)r, (u8)g, (u8)b));
    }
    si_printf("\n\n");

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, 0));
}
