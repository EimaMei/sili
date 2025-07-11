#define SI_IMPLEMENTATION 1

#include <sili.h>
#include <math.h>
#include <tests/test.h>


siIntern
void TEST_PRINT_EX(siString fmt, siArray(siAny) args, siString expected) {
	siArray(u8) buffer = SI_ARR_STACK(SI_KILO(8));
	ASSERT(expected.len <= buffer.len);

	siString received = SI_STR_LEN(buffer.data, si_bprintfEx(buffer, fmt, args));
	TEST_EQ_STR(expected, received);
}

#define TEST_PRINT(expected, fmt, ...) TEST_PRINT_EX(SI_STR(fmt), SI_ARGS(__VA_ARGS__), SI_STR(expected)); SUCCEEDED()
#define TEST_PRINT_REG(expected) TEST_PRINT(expected, "", SI_STR(expected))

int main(void) {
	/* TODO(EimaMei): Make C-string string literals somehow usable and not make
	 * the app crash. */
	TEST_START();

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

	TEST_PRINT("Characters:\tA % ė\n", "Characters:\t%c %% %c\n", 'A', 0x0117);

	TEST_PRINT_REG("Integers:\n");
	TEST_PRINT(
		"\tDecimal:\t1 2 000003 0  +4 -4\n",
		"\tDecimal:\t%i %d %.6i %i %.0i %+i %i\n",
					  1, 2,   3, 0,   0, +4, -4
	);

	TEST_PRINT("\tHexadecimal:\t5 a A 0x6\n", "\tHexadecimal:\t%x %x %X %#x\n", 5, 10, 10, 6);
	TEST_PRINT("\tOctal:\t\t12 0o12 0o4\n","\tOctal:\t\t%o %#o %#o\n", 10, 10, 4);
	TEST_PRINT_REG("Floating-point:\n");
	// TODO(EimaMei): Float stuff is inaccurate and not that efficient. needs a rework.
	//TEST_PRINT("\tRounding:\t1.500000 2 1.30000000000000000000000000000000\n", "\tRounding:\t%f %.0f %.32f\n", 1.5, 1.5, 1.3);
	//TEST_PRINT("\tPadding:\t01.50 1.50  1.50\n", "\tPadding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
	//TEST_PRINT("\tScientific:\t1.500000E+00 1.500000e+00\n", "\tScientific:\t%E %e\n", 1.5, 1.5);
	/* TEST_PRINT("\t0x1.8p+0 0X1.8P+0\n", "\tHexadecimal:\t%a %A\n", 1.5, 1.5); */
	{
		const f64 nan = (f64)NAN;
		const f64 inf = (f64)INFINITY;
		const f64 n_inf = (f64)-INFINITY;
		TEST_PRINT("\tSpecial values:\t0/0=nan 1/0=inf -1/0=-inf\n", "\tSpecial values:\t0/0=%g 1/0=%g -1/0=%g\n", nan, inf, n_inf);
	}

	TEST_PRINT_REG("Fixed-width types:\n");
	TEST_PRINT("\tLargest 32-bit value is 4294967295 or 0xffffffff\n", "\tLargest 32-bit value is %i or %#x\n", UINT32_MAX, UINT32_MAX);
	TEST_PRINT("true false 0b1 0b0\n", "%t %t %#b %#b\n", true, false, 1, 0);
	TEST_PRINT("qwertyuiop\n", "%s\n", SI_STR("qwertyuiop"));

	si_printLn(SI_STR("================\nPrint colour tests:\nANSI/3-bit colour:"));
	for_range (id, siPrintColor3bit_Black, (u8)8) {
		siPrintColor clr = si_printColor3bit(id),
					 bold = si_printColor3bit(id, siPrintColorAnsiBits_Bold),
					 light = si_printColor3bit(id, siPrintColorAnsiBits_Light),
					 both = si_printColor3bit(id, siPrintColorAnsiBits_Bold | siPrintColorAnsiBits_Light);
		si_printfLn("\t%CColor %i:%C %CBold%C %CLight%C %CAll%C", clr, id, bold, light, both);
	}

	si_printLn(SI_STR("\n8-bit colour:\n\t"));
	for_range (i, 0, UINT8_MAX + 1) {
		siPrintColor clr = si_printColor8bit((u8)i);
		si_printf("%C% 3i%C ", clr, i);

        if (i == 15 || (i > 15 && (i - 15) % 6 == 0)) {
            si_print(SI_STR("\n\t"));
        }
	}
	si_printf("\n24-bit colour (%t):\n\t", si_printHas24bitColor());

    for_range (column, 0, 77) {
        isize r = 255 - (column * 255 / 76);
        isize g = (column * 510 / 76);
        isize b = (column * 255 / 76);

        if (g > 255) {
            g = 510 - g;
        }

		/* TODO(EimaMei): On Windows a newline appears after each print for whatever reason. */
		/* NOTE(EimaMei, 2025-07-11): It seems the newline issue dissapears on
		 * windows if you use nmake strangely enough. */
		si_printf("%C0%C", si_printColor24bit((u8)r, (u8)g, (u8)b));
    }
    si_print(SI_STR("\n\n"));

	TEST_COMPLETE();
}
