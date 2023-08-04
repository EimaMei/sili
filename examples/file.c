#define SI_IMPLEMENTATION 1
#include <sili.h>


void example_4_0(siAllocator* heap) {
    siAllocator* stack = si_allocator_make_stack(4096);

	printf("==============\n\n==============\nExample 4.0:\n");

	siFile file = si_file_open("examples/array.c"); /* If the file doesn't exist or fails to open any other way, then we will get an assertion error. */
	printf(
        "About 'examples/array.c':\n\t"
            "Full path - '%s'\n\t"
            "Size - '%zu' bytes\n",
        si_path_get_fullname(stack, "examples/array.c"), file.size
    );

	siFile new_file = si_file_create("random.txt");
	si_file_write(&new_file, "A silly file\nwith a sili newline.");
    siString content = si_file_read(heap, new_file);

    printf(
        "About 'random.txt':\n\t"
            "Full path - '%s'\n\t"
            "Size - '%zu' bytes\n\t"
            "Content - '%s' (len: '%zd')\n\n",
        si_path_get_fullname(stack, "random.txt"), new_file.size,
        content, si_string_len(content)
    );

	siArray(siString) file_lines = si_file_readlines(heap, file);
	printf(
        "Contents of '%s' ('%zd' lines in total):\n",
        si_path_base_name("array.c"), si_array_len(file_lines)
    );

	for_range (i, 0, si_array_len(file_lines)) {
		si_string_strip(&file_lines[i]);
		printf("\tLine %zd: '%s'\n", i, file_lines[i]);
	}

	si_file_write_at_line(&new_file, "but now we have a changed line", 1);
	siArray(siString) new_file_lines = si_file_readlines(heap, new_file);
	printf(
        "Contents of '%s' ('%zd' lines in total):\n",
        si_path_base_name("random.txt"), si_array_len(new_file_lines)
    );

	for_range (i, 0, si_array_len(new_file_lines)) {
		si_string_strip(&new_file_lines[i]);
		printf("\tLine %zd: '%s'\n", i, new_file_lines[i]);
	}

	si_file_close(file);
	si_file_close(new_file);
}

void example_4_1(void)	{
	printf("==============\n\n==============\nExample 4.1:\n");

	bool exist = si_path_exists("example.c");
	printf("File 'example.c' %s\n", (exist ? "DOES exist" : "DOESN'T exist"));

	exist = si_path_exists("random.txt");
	if (!exist) {
		printf("Since 'random.txt' doesn't exist, we'll just create one\n");

		siFile file = si_file_create("random.txt");
		si_file_write(&file, "KANT RUINED US ALL");
		si_file_close(file);
	}

	bool res = si_path_copy("random.txt", "random-2.txt");
	printf(
        "Does 'random-2.txt' exist: '%zd' (res: '%zd')\n",
        si_path_exists("random-2.txt"), res
    );

	res = si_path_move("random.txt", "renamed.txt");
	printf(
        "Does 'random.txt' exist:'%zd'\n'renamed.txt' outputs a '%zd' (res: '%zd')\n",
        si_path_exists("random.txt"), si_path_exists("renamed.txt"), res
    );

    siAllocator* stack = si_allocator_make(4096);

    const char* path = "example.c";
	siString full_path = si_path_get_fullname(stack, path);
	printf(
        "Information about '%s':\n\t"
            "Base name - '%s'\n\t"
            "Extension - '%s'\n\t"
            "Full path - '%s'\n\t"
            "Is relative: %zd\n",
        path, si_path_base_name(path), si_path_extension(path),
        full_path, si_path_is_relative(path)
    );

	res = si_path_remove("random-2.txt");
	printf(
        "Does 'random-2.txt' exist: '%zd' (res: '%zd')\n",
        si_path_exists("random-2.txt"), res
    );

	res = si_path_remove("renamed.txt");
	printf(
        "Does 'renamed.txt' exist: '%zd' (res: '%zd')\n",
        si_path_exists("renamed.txt"), res
    );
}


int main(void) {
    siAllocator* heap = si_allocator_make(SI_KILO(8));

    example_4_0(heap);
    example_4_1();

    si_allocator_free(heap);
	return 0;
}
