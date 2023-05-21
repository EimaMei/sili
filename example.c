#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <stdio.h>

#define EXAMPLES_1_PLUS 1
#define EXAMPLES_2_PLUS 1
#define EXAMPLES_3_PLUS 1


int main() {
	#if EXAMPLES_1_PLUS == 1
	/* Example 1.0: General string usage */
	{
		printf("==============\nExample 1.0:\n");

		siString str = si_string_make("Labas, Pasauli!");
		printf("str: %s\n", str);

		si_string_append(&str, " Lithuanian, more like Russian amirite.");
		printf("str: %s\n", str);

		char front = si_string_front(str);
		char back = si_string_back(str);
		usize length = si_string_len(str);
		printf("front: '%c', back: '%c', len: '%ld'\n", front, back, length);

		si_string_set(&str, "Different sentence");
		printf("str: %s\n", str);

		si_string_push_back(&str, '.');
		printf("str: %s\n", str);

		siString str2 = si_string_copy(str);
		bool result = si_strings_are_equal(str, str2);
		printf("(\"%s\" == \"%s\") returns a '%s' boolean\n", str, str2, (result ? "true" : "false"));

		isize pos = si_string_find(str, "sentence");
		printf("The word 'sentence' was found at position %ld (Starting with the letter '%c')\n", pos, str[pos]);

		pos = si_string_find(str, "random");
		printf("However, the word 'random' was not found, thus 'pos' equals to %ld\n", pos);

		si_string_replace(&str, "Different", "Completely new");
		printf("str: %s\n", str);

		si_string_replace(&str2, "Different", "The same");
		printf("str2: %s\n", str2);

		si_string_trim(&str, "sentence.");
		printf("str: %s\n", str);

		si_string_clear(&str);
		printf("Length of str: %ld\n", si_string_len(str));


		si_string_set(&str2, "one.two.three.four.five");
		printf("Current str2: %s %li\n", str2, si_string_len(str2));

		siArray(siString) list = si_string_split(str2, ".");
		printf("hi %p\n", list);
		usize i;
		for (i = 0; i < si_array_len(list); i++) {
			printf("Element %ld: '%s'\n", i, list[i]);
		}
		printf("what\n");

		si_string_free(str);
		printf("what\n");
		si_string_free(str2);
		printf("what\n");
	}

	/* Example 1.1: Other usages. */
	{
		printf("==============\n\n==============\nExample 1.1:\n");

		/* Int stuff: */
		siString str = si_string_make_int(-342);
		printf("str: \"%s\"\n", str);

		isize num = si_cstr_to_int("9300");
		printf("num: %ld\n", num);


		/* Join, upper, lower, title, capitalize */
		si_string_set(&str, "/home");
		printf("Original str: \"%s\"\n", str);

		si_string_join(&str, "random.txt", "/");
		printf("Joined str: \"%s\"\n", str);

		si_string_upper(&str);
		printf("Upper str: \"%s\"\n", str);

		si_string_set(&str, "I'VE COME TO MAKE AN ANNOUNCEMENT");
		printf("Original str: \"%s\"\n", str);

		si_string_lower(&str);
		printf("Lower str: \"%s\"\n", str);

		si_string_title(&str);
		printf("Titled str: \"%s\"\n", str);

		si_string_capitalize(&str);
		printf("Capitalized str: \"%s\"\n", str);


		si_string_free(str);
	}
	#endif

	#if EXAMPLES_2_PLUS == 1
	/* Example 2.0: General array usage. */
	{
		printf("==============\n\n==============\nExample 2.0:\n");

		siArray(i32) array = si_array_make((i32[]){3, 2, 4, 234, 294});

		usize count = 0;
		foreach (num, array) {
			printf("Element %ld: %i\n", count, *num);
			count++;
		}

		isize value = si_array_find(array, 234);
		printf("Number '234' is at: array[%ld]\n", value);

		usize previous_len = si_array_len(array);
		si_array_append(&array, SI_INT32_MAX); /* si_array_push_back does the same thing. */

		i32* front_ptr = si_array_front(array).ptr;
		i32* back_ptr = si_array_back(array).ptr;
		printf("We now have %d elements instead of %ld. The front value is '%i', while the back value is '%X'\n", si_array_len(array), previous_len, *front_ptr, *back_ptr);

		isize element_pos = si_array_replace(&array, 4, SI_INT32_MIN);
		printf("The element at position '%ld' was replaced with: %X\n", element_pos, array[element_pos]);

		siArray(i32) copy = si_array_copy(array);
		bool res = si_arrays_are_equal(array, copy);

		printf("Arrays 'array' and 'copy' are %s\n", (res ? "the same" : "NOT the same"));

		si_array_free(array);
		si_array_free(copy);
	}
	#endif

	#if EXAMPLES_3_PLUS == 1
	/* Example 3.0: siPair (based on https://cplusplus.com/reference/utility/pair/pair/) */
	{
		printf("==============\n\n==============\nExample 3.0:\n");

		siPair(siString, f64) product1;
  		siPair(siString, f64) product2 = si_pair_make(si_string_make("tomatoes"), 2.30);
  		siPair(siString, f64) product3 = si_pair_copy(product2);
  		siPair(siString, f64) product4 = {si_string_make("bread"), 599.99};

		product1.first = si_string_make("shoes");
		product1.second = 39.90;

		printf("The price of %s is $%f\n", product1.first, product1.second);
		printf("The price of %s is $%f\n", product2.first, product2.second);
		printf("The price of %s is $%f\n", product3.first, product3.second);
		printf("The price of %s is $%f\n", product4.first, product4.second);

		si_string_free(product1.first);
		si_string_free(product2.first);
		si_string_free(product4.first);
	}
	#endif

    return 0;
}