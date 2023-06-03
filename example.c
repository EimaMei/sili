#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <stdio.h>

#define EXAMPLE_SI_ENABLE_ALL    1

#define EXAMPLE_SI_STRING        0
#define EXAMPLE_SI_ARRAY         0
#define EXAMPLE_SI_PAIR          0
#define EXAMPLE_SI_FILE          0
#define EXAMPLE_SI_OPTIONAL      0
#define EXAMPLE_SI_THREAD        0
#define EXAMPLE_SI_PERFORMANCE   0

#if EXAMPLE_SI_ENABLE_ALL == 1
	#undef EXAMPLE_SI_STRING
	#undef EXAMPLE_SI_ARRAY
	#undef EXAMPLE_SI_PAIR
	#undef EXAMPLE_SI_FILE
	#undef EXAMPLE_SI_OPTIONAL
	#undef EXAMPLE_SI_THREAD
	#undef EXAMPLE_SI_PERFORMANCE

	#define EXAMPLE_SI_STRING     1
	#define EXAMPLE_SI_ARRAY      1
	#define EXAMPLE_SI_PAIR       1
	#define EXAMPLE_SI_FILE       1
	#define EXAMPLE_SI_OPTIONAL   1
	#define EXAMPLE_SI_THREAD     1
	#define EXAMPLE_SI_PERFORMANCE 1
#endif

#if EXAMPLE_SI_OPTIONAL == 1
	siOptional(char*) create(bool value) {
		return (value ? si_optional_make((char*)"Godzilla") : SI_OPTIONAL_NULL);
	}
#endif

#if EXAMPLE_SI_THREAD == 1
	rawptr thread_test(bool* arg) {
		bool loop = *arg;
		i16 count = SI_INT16_MIN;

		if (loop) {
			printf("We'll increment 'count' from %d to %d:\n", SI_INT16_MIN, SI_INT16_MAX);
			si_sleep(2000);
			while (count < SI_INT16_MAX) {
				count += 1;
				printf("%i\n", count);
			}
		}
		else {
			printf("'arg' equals to 'false', so I'll just do nothing and wait for like 3 seconds.\n");
			si_sleep(3000);
			printf("...and we're done! Exiting the thread now.\n");
		}

		typeof(count)* res = malloc(sizeof(count));
		SI_ASSERT_NOT_NULL(res);
		*res = count;

		return res;
	}
#endif

#if EXAMPLE_SI_PERFORMANCE == 1
	void performance_test(void) {
		isize i;
		for (i = 0; i < SI_UINT16_MAX; i++);	/* NOTE(EimaMei): Should strain the CPU a little. */
	}
#endif


int main(void) {
	#if EXAMPLE_SI_STRING == 1
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
		printf("front: '%c', back: '%c', len: '%zd'\n", front, back, length);

		si_string_set(&str, "Different sentence");
		printf("str: %s\n", str);

		si_string_push_back(&str, '.');
		printf("str: %s\n", str);

		siString str2 = si_string_copy(str);
		bool result = si_strings_are_equal(str, str2);
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
		}

		si_string_free(str);
		si_string_free(str2);
	}

	/* Example 1.1: int conversions, utilisation of 'char' functions. */
	{
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
	}

	/* Example 1.2: Other. */
	{
		printf("==============\n\n==============\nExample 1.2:\n");

		siString str = si_string_make("\t       dnuora gniliart        ");
		printf("Before: '%s' (len: '%zd')\n", str, si_string_len(str));

		si_string_strip(&str);
		printf("After: '%s' (len: '%zd')\n", str, si_string_len(str));

		si_string_reverse(&str);
		printf("'str' in reverse: '%s'\n", str);

		si_string_free(str);
	}
	#endif

	#if EXAMPLE_SI_ARRAY == 1
	/* Example 2.0: General array usage. */
	{
		printf("==============\n\n==============\nExample 2.0:\n");

		siArray(i32) array = si_array_make((i32[]){3, 234, 2, 4, 294, 234, 23});

		usize count = 0;
		foreach (num, array) {
			printf("Element %zd: %i\n", count, *num);
			count++;
		}

		isize find_pos = si_array_find(array, 234);
		isize rfind_pos = si_array_rfind(array, 234);
		printf("The 1st number '234' is at 'array[%zd]', while the 2nd one is at 'array[%zd]'\n", find_pos, rfind_pos);

		usize previous_len = si_array_len(array);
		si_array_append(&array, SI_INT32_MAX); /* si_array_push_back does the same thing. */

		i32 front = si_any_get(si_array_front(array), i32);
		i32 back = si_any_get(si_array_back(array), i32);
		printf("We now have %zd elements instead of %zd. The front value is '%i', while the back value is '%X'\n", si_array_len(array), previous_len, front, back);

		isize element_pos = si_array_replace(&array, 4, SI_INT32_MIN);
		printf("The element at position '%zd' was replaced with: %X\n", element_pos, array[element_pos]);

		siArray(i32) copy = si_array_copy(array);
		bool res = si_arrays_are_equal(array, copy);

		printf("Arrays 'array' and 'copy' are %s\n", (res ? "the same" : "NOT the same"));

		si_array_free(array);
		si_array_free(copy);
	}
	/* Example 2.1: Other usages. */
	{
		printf("==============\n\n==============\nExample 2.1:\n");
		siArray(i32) array = si_array_make((i32[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

		printf("Array in regular order: ");
		foreach (num, array) {
			printf("%i ", *num);
		}
		printf("\n");

		si_array_reverse(&array);
		printf("Array in reverse order: ");

		foreach (num, array) {
			printf("%i ", *num);
		}
		printf("\n");
	}
	#endif

	#if EXAMPLE_SI_PAIR == 1
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

	#if EXAMPLE_SI_FILE == 1
	/* Example 4.0: siFile. */
	{
		printf("==============\n\n==============\nExample 4.0:\n");

		siFile file = si_file_open("example.c"); /* If the file doesn't exist or fails to open any other way, then we will get an assertion error. */
		printf("About 'example.c':\n\tFull path - '%s'\n\tSize - '%zu' bytes\n", file.path, file.size);

		siFile new_file = si_file_create("random.txt");
		si_file_write(&new_file, "A silly file\nwith a sili newline.");
		printf("About 'random.txt':\n\tFull path - '%s'\n\tSize - '%zu' bytes\n", file.path, file.size);

		siString content = si_file_read(new_file);
		printf("\tContent - '%s' (len: '%zd')\n", content, si_string_len(content));

		siArray(siString) file_lines = si_file_readlines(file);
		printf("Contents of '%s' ('%zd' lines in total):\n", si_path_base_name(file.path), si_array_len(file_lines));

		usize i;
		for (i = 0; i < si_array_len(file_lines); i++) {
			si_string_strip(&file_lines[i]);
			printf("\tLine %zd: '%s'\n", i, file_lines[i]);
		}

		si_file_write_at_line(&new_file, "but now we have a changed line\n", 1);
		siArray(siString) new_file_lines = si_file_readlines(new_file);
		printf("Contents of '%s' ('%zd' lines in total):\n", si_path_base_name(new_file.path), si_array_len(new_file_lines));

		for (i = 0; i < si_array_len(new_file_lines); i++) {
			si_string_strip(&new_file_lines[i]);
			printf("\tLine %zd: '%s'\n", i, new_file_lines[i]);
		}

		si_string_free(content);

		si_array_free(file_lines);
		si_array_free(new_file_lines);

		si_file_close(file);
		si_file_close(new_file);
	}
	/* Example 4.1: 'si_path' functions. */
	{
		printf("==============\n\n==============\nExample 4.1:\n");

		bool exist = si_path_exists("example.c");
		printf("File 'example.c' %s\n", (exist ? "DOES exist" : "DOESN'T exist"));

		exist = si_path_exists("random.txt");
		if (!exist) {
			printf("Since 'random.txt' doesn't exist, we'll just create one\n");

			siFile file = si_file_open_mode("random.txt", SI_FILE_MODE_CREATE);
			si_file_write(&file, "Creating files is too easy tbh.");
			si_file_close(file);
		}

		bool res = si_path_copy("random.txt", "random-2.txt");
		printf("Does 'random-2.txt' exist: '%zd' (res: '%zd')\n", si_path_exists("random-2.txt"), res);

		res = si_path_move("random.txt", "renamed.txt");
		printf("Does 'random.txt' exist: '%zd', but 'renamed.txt' outputs a '%zd' (res: '%zd')\n", si_path_exists("random.txt"), si_path_exists("renamed.txt"), res);

		const char* path = "example.c";
		siString full_path = si_path_get_fullname(path);
		printf("Information about '%s':\n\tBase name - '%s'\n\tExtension - '%s'\n\tFull path - '%s'\n\tIs relative: %zd\n", path, si_path_base_name(path), si_path_extension(path), full_path, si_path_is_relative(path));

		res = si_path_remove("random-2.txt");
		printf("Does 'random-2.txt' exist: '%zd' (res: '%zd')\n", si_path_exists("random-2.txt"), res);

		res = si_path_remove("renamed.txt");
		printf("Does 'renamed.txt' exist: '%zd' (res: '%zd')\n", si_path_exists("renamed.txt"), res);

		si_string_free(full_path);
	}

	#endif

	#if EXAMPLE_SI_OPTIONAL == 1
	/* Example 5.0: siOptional (based on https://en.cppreference.com/w/cpp/utility/optional). */
	{
		printf("==============\n\n==============\nExample 5.0:\n");

		printf("create(false) returned '%s'\n", si_optional_if_no_value(create(false), (char*)"empty"));

		siOptional(char*) str = create(true);
		printf("create2(true) returned '%s'\n", si_any_get(str.value, char*));
	}
	#endif

	#if EXAMPLE_SI_THREAD == 1
		siThread thread = si_thread_create(siFunc(thread_test), &(bool){false});
		si_thread_start(&thread);

		while (thread.is_running) {
			printf("Even though 'thread' is currently sleeping, it's still running this exact second!\n");
			si_sleep(1000);
		}
		printf("That loop returned a '%i'. Now we'll re-run the loop with the argument being 'true' instead.\n", *((i16*)thread.return_value));
		free(thread.return_value);
		si_sleep(2000);

		thread.arg = &(bool){true};
		si_thread_start(&thread);
		si_thread_join(&thread); /* Now we have to wait... */

		printf("That loop NOW returned a '%i'.\n", *((i16*)thread.return_value));
		free(thread.return_value);
		si_sleep(2000);

		#if !defined(SI_SYSTEM_WINDOWS)
			si_thread_start(&thread);
			si_sleep(2500);
			si_thread_cancel(&thread);
			printf("Decided to kill it 2.5 seconds later.\n");
		#endif
	#endif

	#if EXAMPLE_SI_PERFORMANCE == 1

	printf("Running 'performance_test()' 30000 times. Lets see how long it takes to execute that many times...\n");
	si_performance_loop(30000, performance_test());

	printf("Now lets see how many times 'performance_test()' can be excuted in 5 seconds...\n");
	si_performance_exec_per_ms(5000, performance_test());

	#endif

    return 0;
}
