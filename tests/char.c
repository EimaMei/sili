#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>

SI_STATIC_ASSERT(SI_ASCII_MAX == 0x7F);

int main(void) {
	TEST_START();
	static const char expected_lower[0x80] = {
	    '\0', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
	    '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
	    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
	    '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
	    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
	    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
	    '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '\\', ']', '^', '_',
	    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '\x7F'
	};

	static const char expected_upper[0x80] = {
	    '\0', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
	    '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
	    '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
	    '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
	    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
	    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
	    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
	    '`', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', '~', '\x7F'
	};

	for_range (i, 0, SI_ASCII_MAX + 1) {
		char x = (char)i;
		char res_lower = si_charLower(x),
			 res_upper = si_charUpper(x);

		b32 bool_lower = si_charIsLower(x),
			bool_upper = si_charIsUpper(x),
			bool_space = si_charIsSpace(x),
			bool_digit = si_charIsDigit(x),
			bool_hex   = si_charIsHex(x),
			bool_alpha = si_charIsAlpha(x),
			bool_alphanum = si_charIsAlphanumeric(x),
			bool_punc = si_charIsPunctuation(x),
			bool_ctrl = si_charIsControl(x),
			bool_prnt = si_charIsPrintable(x),
			bool_grap = si_charIsGraphical(x),
			bool_deli = si_charIsDelimiter(x);

		i32 conv_digit = si_charDigitToInt(x),
			conv_hex = si_charBase32ToInt(x);

		//si_printf("%i: '%c' '%c' '%c'\n", x, x, res_lower, expected_lower[i]);
		TEST_EQ_CHAR(res_upper, expected_upper[i]);
		TEST_EQ_CHAR(res_lower, expected_lower[i]);

		TEST_EQ_U32(si_between(i8, x, 'A', 'Z'), bool_upper);
		TEST_EQ_U32(si_between(i8, x, 'a', 'z'), bool_lower);
		TEST_EQ_U32(si_between(i8, x, '\t', '\r') || x == ' ', bool_space);
		TEST_EQ_U32(si_between(i8, x, '0', '9'), bool_digit);
		TEST_EQ_U32(si_between(i8, x, '0', '9') || si_between(i8, x, 'a', 'f') || si_between(i8, x, 'A', 'F'), bool_hex);
		TEST_EQ_U32(si_between(i8, x, 'a', 'z') || si_between(i8, x, 'A', 'Z'), bool_alpha);
		TEST_EQ_U32(si_between(i8, x, 'a', 'z') || si_between(i8, x, 'A', 'Z') || si_between(i8, x, '0', '9'), bool_alphanum);
		TEST_EQ_U32(si_between(i8, x, '!', '/') || si_between(i8, x, ':', '@') || si_between(i8, x, '[', '`') || si_between(i8, x, '{', '~'), bool_punc);
		TEST_EQ_U32(si_between(i8, x, '\0', '\x1F') || x == '\x7F', bool_ctrl);
		TEST_EQ_U32(bool_alphanum || bool_punc || bool_space, bool_prnt);
		TEST_EQ_U32(bool_alphanum || bool_punc, bool_grap);

		TEST_EQ_U32(bool_alphanum || x == '@' || x == '#' || x == '$', !bool_deli);

		if (conv_digit != -1) {
			TEST_EQ_CHAR(conv_digit, (x - '0'));
		}
		else {
			TEST_EQ_U32(bool_digit, false);
		}

		if (conv_hex != -1) {
			// TEST_EQ_U32(bool_hex, true);
			if (bool_digit) {
				TEST_EQ_CHAR(conv_hex, (x  - '0'));
			} else if (bool_upper) {
				TEST_EQ_CHAR(conv_hex, (x  - 'A' + 10));
			} else if (bool_lower) {
				TEST_EQ_CHAR(conv_hex, (x  - 'a' + 10));
			}
		}
		else {
			SI_ASSERT(!bool_hex);
		}
	}

	TEST_COMPLETE();
}
