#define SI_IMPLEMENTATION 1
#include <sili.h>


void example_4_0(void) {
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
		si_string_free(file_lines[i]);
	}

	si_file_write_at_line(&new_file, "but now we have a changed line\n", 1);
	siArray(siString) new_file_lines = si_file_readlines(new_file);
	printf("Contents of '%s' ('%zd' lines in total):\n", si_path_base_name(new_file.path), si_array_len(new_file_lines));

	for (i = 0; i < si_array_len(new_file_lines); i++) {
		si_string_strip(&new_file_lines[i]);
		printf("\tLine %zd: '%s'\n", i, new_file_lines[i]);
		si_string_free(new_file_lines[i]);
	}

	//si_string_free(content);

	si_array_free(file_lines);
	si_array_free(new_file_lines);

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


int main(void) {
    example_4_0();
    example_4_1();

    return 0;
}
