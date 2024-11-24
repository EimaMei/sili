#define SI_IMPLEMENTATION 1
#include <sili.h>

int main(void) {
	SI_STATIC_ASSERT(SI_ASCII_MAX == 0x7F);

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
			conv_hex = si_charHexToInt(x);

		si_printf("%i: '%c' '%c' '%c'\n", x, x, res_lower, expected_lower[i]);
		SI_ASSERT(res_upper == expected_upper[i]);
		SI_ASSERT(res_lower == expected_lower[i]);

		if (bool_upper == true) {
			SI_ASSERT(si_between(i8, x, 'A', 'Z'));
		}
		else if (bool_lower == true) {
			SI_ASSERT(si_between(i8, x, 'a', 'z'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, 'A', 'Z') && !si_between(i8, x, 'a', 'z'));
			SI_ASSERT(bool_upper == false && bool_lower == false);
		}

		if (bool_space) {
			SI_ASSERT(si_between(i8, x, '\t', '\r') || x == ' ');
		}
		else {
			SI_ASSERT(!si_between(i8, x, '\t', '\r') && x != ' ');
		}

		if (bool_digit) {
			SI_ASSERT(si_between(i8, x, '0', '9'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, '0', '9'));
		}

		if (bool_hex) {
			SI_ASSERT(si_between(i8, x, '0', '9') || si_between(i8, x, 'a', 'f') || si_between(i8, x, 'A', 'F'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, '0', '9') && !si_between(i8, x, 'a', 'f') && !si_between(i8, x, 'A', 'F'));
		}

		if (bool_alpha) {
			SI_ASSERT(si_between(i8, x, 'a', 'z') || si_between(i8, x, 'A', 'Z'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, 'a', 'z') && !si_between(i8, x, 'A', 'Z'));
		}

		if (bool_alphanum) {
			SI_ASSERT(si_between(i8, x, 'a', 'z') || si_between(i8, x, 'A', 'Z') || si_between(i8, x, '0', '9'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, 'a', 'z') && !si_between(i8, x, 'A', 'Z') && !si_between(i8, x, '0', '9'));
		}

		if (bool_punc) {
			SI_ASSERT(si_between(i8, x, '!', '/') || si_between(i8, x, ':', '@') || si_between(i8, x, '[', '`') || si_between(i8, x, '{', '~'));
		}
		else {
			SI_ASSERT(!si_between(i8, x, '!', '/') && !si_between(i8, x, ':', '@') && !si_between(i8, x, '[', '`') && !si_between(i8, x, '{', '~'));
		}

		if (bool_ctrl) {
			SI_ASSERT(si_between(i8, x, '\0', '\x1F') || x == '\x7F');
		}
		else {
			SI_ASSERT(!si_between(i8, x, '\0', '\x1F') && x != '\x7F');
		}

		if (bool_prnt) {
			SI_ASSERT(bool_alphanum || bool_punc || bool_space);
		}
		else {
			SI_ASSERT(!bool_alphanum && !bool_punc && !bool_space);
		}

		if (bool_grap) {
			SI_ASSERT(bool_alphanum || bool_punc);
		}
		else {
			SI_ASSERT(!bool_alphanum && !bool_punc);
		}

		if (bool_deli) {
			SI_ASSERT(!bool_alphanum && x != '@' && x != '#' && x != '$');
		}
		else {
			SI_ASSERT(bool_alphanum || x == '@' || x == '#' || x == '$');
		}

		if (conv_digit != -1) {
			SI_ASSERT(bool_digit && conv_digit == (x - '0'));
		}
		else {
			SI_ASSERT(!bool_digit);
		}

		if (conv_hex != -1) {
			SI_ASSERT(bool_hex && ((bool_digit && conv_hex == (x  - '0')) || (bool_upper && conv_hex == (x - 'A' + 10)) || (bool_lower && conv_hex == (x - 'a' + 10))));
		}
		else {
			SI_ASSERT(!bool_hex);
		}
	}

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColorAnsi_Yellow, true, false));
}
