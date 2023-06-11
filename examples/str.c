#define SI_IMPLEMENTATION 1
#include <sili.h>


void example_1_0(void) {
    printf("==============\nExample 1.0:\n");

	siString str = si_string_make("Labas, Pasauli!");
	printf("str: %s\n", str);

	si_string_append(&str, " Lithuanian, more like Russian amirite.");
	printf("str: %s\n", str);

	char front = si_string_front(str);
	char back = si_string_back(str);
	usize length = si_string_len(str);
	printf("front: '%c', back: '%c', len: '%zd'\n", front, back, length);

	si_string_set(&str, "Different sentence");
	printf("str: %s\n", str);

	si_string_push(&str, '.');
	printf("str: %s\n", str);

	siString str2 = si_string_copy(str);
	bool result = si_cstr_equal(str, str2);
	printf("(\"%s\" == \"%s\") returns a '%s' boolean\n", str, str2, (result ? "true" : "false"));

	isize pos = si_string_find(str, "sentence");
	printf("The word 'sentence' was found at position '%zd' (Starting with the letter '%c')\n", pos, str[pos]);

	pos = si_string_find(str, "random");
	printf("However, the word 'random' was not found, thus 'pos' equals to %zd\n", pos);

	si_string_replace(&str, "Different", "Completely new");
	printf("str: %s\n", str);

	si_string_replace(&str2, "Different", "The same");
	printf("str2: %s\n", str2);

	si_string_trim(&str, "sentence.");
	printf("str: %s\n", str);

	si_string_clear(&str);
	printf("Length of str: %zd\n", si_string_len(str));


	si_string_set(&str2, "one.two.three.four.five");
	printf("Current str2: %s\n", str2);

	siArray(siString) list = si_string_split(str2, ".");
	usize i;
	for (i = 0; i < si_array_len(list); i++) {
		printf("\tElement %zd: '%s'\n", i, list[i]);
		si_string_free(list[i]);
	}

	si_string_free(str);
	si_string_free(str2);
	si_array_free(list);
}

void example_1_1(void) {
    printf("==============\n\n==============\nExample 1.1:\n");
	/* Int stuff: */
	siString str = si_string_make(si_i64_to_cstr(-342));
	printf("str: \"%s\"\n", str);

	isize num = si_cstr_to_u64("9300");
	printf("num: %zd\n\n", num);


	/* Join, upper, lower, title, capitalize */
	si_string_set(&str, "/home");
	printf("Original str: \"%s\"\n", str);

	si_string_join(&str, "random.txt", "/");
	printf("Joined str: \"%s\"\n", str);

	si_cstr_upper(str);
	printf("Upper str: \"%s\"\n", str);

	si_string_set(&str, "I'VE COME TO MAKE AN ANNOUNCEMENT");
	printf("Original str: \"%s\"\n", str);

	si_cstr_lower(str);
	printf("Lower str: \"%s\"\n", str);

	si_cstr_title(str);
	printf("Titled str: \"%s\"\n", str);

	si_cstr_capitalize(str);
	printf("Capitalized str: \"%s\"\n", str);

	si_string_free(str);
}

void example_1_2(void) {
	printf("==============\n\n==============\nExample 1.2:\n");
	siString str = si_string_make("\t       dnuora gniliart        ");
	printf("Before: '%s' (len: '%zd')\n", str, si_string_len(str));
	si_string_strip(&str);
	printf("After: '%s' (len: '%zd')\n", str, si_string_len(str));
	si_string_reverse(&str);
	printf("'str' in reverse: '%s'\n", str);
	si_string_free(str);
}


int main(void) {
    example_1_0();
    example_1_1();
    example_1_2();

    return 0;
}
