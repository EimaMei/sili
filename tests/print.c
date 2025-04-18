#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <math.h>
#include <tests/test.h>

siIntern
void TEST_PRINT_EX(cstring expectedStr, cstring fmt, siCallerLoc loc, ...) {
	siArray(u8) buffer = SI_ARR_STACK(1024);

	siString in = SI_CSTR(expectedStr);
	ASSERT(in.len <= buffer.len);

	siString out;
	{
		va_list va;
		va_start(va, loc);
		out = si_bprintfVa(buffer, SI_CSTR(fmt), va);
		va_end(va);
	}

	TEST_EQ_STR(out, in);
}

#define TEST_PRINT(in, fmt, ...) TEST_PRINT_EX(in, fmt, SI_CALLER_LOC, __VA_ARGS__); SUCCEEDED()
#define TEST_PRINT_REG(expectedStr) TEST_PRINT(expectedStr, expectedStr, 0)


int main(void) {
	TEST_START();

	/* From: https://en.cppreference.com/w/c/io/fprintf */
	const char* str = "Hello";
	TEST_PRINT_REG("Strings:\n");
	TEST_PRINT_REG(" padding:\n");
	TEST_PRINT("\t[Hello]\n", "\t[%S]\n", str);
	TEST_PRINT("\t[     Hello]\n", "\t[%10S]\n", str);
	TEST_PRINT("\t[Hello     ]\n", "\t[%-10S]\n", str);
	TEST_PRINT("\t[     Hello]\n", "\t[%*S]\n", 10, str);
	TEST_PRINT_REG(" truncating:\n");
	TEST_PRINT("\tHell\n", "\t%.4S\n", str);
	TEST_PRINT("\tHel\n", "\t%.*S\n", 3, str);

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
	TEST_PRINT("\tRounding:\t1.500000 2 1.30000000000000000000000000000000\n", "\tRounding:\t%f %.0f %.32f\n", 1.5, 1.5, 1.3);
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
	TEST_PRINT("true false 0b1 0b0\n", "%t %t %#b %#b\n", true, false, 1, 0);
	TEST_PRINT("qwertyuiop\n", "%s\n", SI_STR("qwertyuiop"));

	si_printLn("================\nPrint colour tests:\nANSI/3-bit colour:");
	for_rangeEx (u8, id, siPrintColor3bit_Black, siPrintColor3bit_White + 1) {
		siPrintColor clr = si_printColor3bit(id),
					 bold = si_printColor3bitEx(id, true, false),
					 light = si_printColor3bitEx(id, false, true),
					 both = si_printColor3bitEx(id, true, true);
		si_printfLn("\t%CColor %i:%C %CBold%C %CLight%C %CAll%C", clr, id, bold, light, both);
	}

	si_printLn("\n8-bit colour:\n\t");
	for_range (i, 0, UINT8_MAX + 1) {
		siPrintColor clr = si_printColor8bit((u8)i);
		si_printf("%C% 3i%C ", clr, i);

        if (i == 15 || (i > 15 && (i - 15) % 6 == 0)) {
            si_print("\n\t");
        }
	}
	si_printf("\n24-bit colour (%t):\n\t", si_printHas24bitColor());

    for_rangeEx (i32, column, 0, 77) {
        i32 r = 255 - (column * 255 / 76);
        i32 g = (column * 510 / 76);
        i32 b = (column * 255 / 76);

        if (g > 255) {
            g = 510 - g;
        }

		/* TODO(EimaMei): On Windows a newline appears after each print for whatever reason. */
		si_printf("%C0%C", si_printColor24bit((u8)r, (u8)g, (u8)b));
    }
    si_print("\n\n");

	TEST_COMPLETE();
}
