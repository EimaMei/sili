#define SI_IMPLEMENTATION 1
#include <sili.h>


/* Showcases most of the primary 'si_file*' functions. */
void example1(siAllocator alloc);
/* Showcases most of the primary 'si_path*' functions. */
void example2(void);
/* Showcases some of the miscellaneous path and file functions. */
void example3(void);
/* Showcases the usage of 'siDirectory' as well as sili's capabilites to decode
 * unicode file paths. */
void example4(void);
/* Showcases standard and sili ways of using the library's printf function. */
void example5(siAllocator* alloc);


int main(void) {
	siAllocatorData inData;
	siAllocator alloc = si_allocatorMakeArena(SI_MEGA(1), &inData);

	example1(alloc);
	example2();
	example3();
	example4();
	example5(&alloc);

	si_freeAll(alloc);
	return 0;
}


void example1(siAllocator alloc) {
	siAllocator stack = si_allocatorMakeStack(SI_KILO(4));

	si_printf("==============\n\n==============\nExample 1:\n");
	siString file_random = SI_STR("random.txt");
	siString file_examples_file = SI_STR("examples/file.c");
	siString unknown = SI_STRC("N/A");

	/* If the file doesn't exist or fails to open in any other way, then we will
	 * get an assertion error. */
	{
		siFile newFile = si_fileCreate(file_random);
		si_fileWrite(&newFile, SI_STR("A silly file\nwith three sili newlines\nbut not much else."));

		siString content = si_fileReadContents(newFile, alloc);
		si_stringReplaceAll(&content, SI_STR("\n"), SI_STR("\\n"), alloc);

		si_printf(
			"About 'random.txt':\n\t"
				"Full path - '%S'\n\t"
				"Size - '%zu' bytes\n\t"
				"Content - '%S'\n\n",
			si_optionalGetOrDefault(si_pathGetFullName(file_random, stack), unknown),
			content.len, content
		);

		si_fileWriteAtLine(&newFile, SI_STR("and now we have a changed line"), 1);

		siArray(siString) newFileLines = si_fileReadlines(newFile, alloc);
		si_printf(
			"Contents of '%S' ('%zd' lines in total):\n",
			si_pathBaseName(file_random), newFileLines.len
		);

		for_eachArr (siString, str, newFileLines) {
			si_stringStrip(str);
			si_printf("\tLine %zu: '%S'\n", str - (siString*)newFileLines.data, *str);
		}
		si_fileClose(newFile);
	}

	{
		siFile file = si_fileOpen(file_examples_file);
		si_printf(
			"About 'examples/file.c':\n\t"
				"Full path - '%S'\n\t"
				"Size - '%zu' bytes\n",
			si_optionalGetOrDefault(si_pathGetFullName(file_examples_file, stack), unknown),
			file.size
		);

		siArray(siString) lines = si_fileReadlines(file, alloc);
		si_printf(
			"Contents of '%S' ('%zd' lines in total):\n",
			si_pathBaseName(file_examples_file), lines.len
		);

		for_eachArr (siString, line, lines) {
			si_printf("\tLine %zu (%zu bytes): '%S'\n", line - (siString*)lines.data, line->len, *line);
		}
		si_fileClose(file);
	}
}

void example2(void)	{
	siAllocator stack = si_allocatorMakeStack(SI_KILO(4));
	si_printf("==============\n\n==============\nExample 2:\n");

	{
		siString str_random = SI_STR("random.txt"),
				 str_random2 = SI_STR("random-2.txt"),
				 str_renamed = SI_STR("renamed.txt");

		b32 exist = si_pathExists(str_random);
		if (!exist) {
			si_print("Since 'random.txt' doesn't exist, we'll just create one\n");

			siFile file = si_fileCreate(str_random);
			si_fileWrite(&file, SI_STR("QWERTYUIOP"));
			si_fileClose(file);
		}

		siResult(usize) error = si_pathCopy(str_random, str_random2);
		si_printf(
			"Does 'random-2.txt' exist: %B (returned bytes: '%zi')\n",
			si_pathExists(str_random2), si_optionalGetOrDefault(error, 0)
		);

		siError res = si_pathMove(str_random, str_renamed);
		si_printf(
			"Does 'random.txt' exist: %B\n"
			"'renamed.txt' outputs a %B (res: '%i')\n",
			si_pathExists(str_renamed), si_pathExists(str_renamed), res.code
		);

		res = si_pathRemove(str_random2);
		si_printf(
			"Does 'random-2.txt' exist: '%B' (res: '%i')\n",
			si_pathExists(str_random2), res.code
		);

		res = si_pathRemove(str_renamed);
		si_printf(
			"Does 'renamed.txt' exist: '%B' (res: '%i')\n",
			si_pathExists(str_renamed), res.code
		);
	}

	{
		siString path = SI_STR("example.c");
		siOption(siString) fullPath = si_pathGetFullName(path, stack);
		si_printf(
			"Information about '%S':\n\t"
				"Base name - '%S'\n\t"
				"Extension - '%S'\n\t"
				"Full path - '%S'\n\t"
				"Is relative: %B\n",
			path, si_pathBaseName(path), si_pathExtension(path),
			si_optionalGetOrDefault(fullPath, path), si_pathIsRelative(path)
		);
	}
}

void example3(void)	{
	si_printf("==============\n\n==============\nExample 3:\n");

	{
		siError res = si_pathRemove(SI_STR("SI_FILE_THAT_DOESNT_EXIST"));
#ifndef SI_NO_ERROR_STRUCT
		si_printf("Error '%S' occurred at \"%s:%i\": '%S'\n", si_pathFsErrorName(res.code), res.filename, res.line, si_pathFsErrorDesc(res.code));
#else
		si_printf("Error '%S' occurred: '%S'\n", si_pathFsErrorName(res.code), si_pathFsErrorDesc(res.code));
#endif
	}
	{
		siString test_folder = SI_STR("testFolder");

		siError res = si_pathCreateFolder(test_folder);
		SI_ASSERT(res.code == 0 || si_pathExists(test_folder));

		siFilePermissions perms = si_pathPermissions(test_folder);
		si_printf("Permissions of 'testFolder' (in octal): %o\n", perms);

		si_pathEditPermissions(test_folder, SI_FS_PERM_ALL);
		perms = si_pathPermissions(test_folder);
		si_printf("Permissions of 'testFolder' (in octal): %o\n", perms);

		si_pathRemove(test_folder);
	}

	{
		u64 lastWriteTime, curWriteTime;
		siString file_path = SI_STR("randomSiFile.silitxt");

		siFile file_handle = si_fileCreate(file_path);

		lastWriteTime = si_fileLastWriteTime(file_handle);
		si_printf("Last write time: %lu\n", lastWriteTime);

		si_sleep(1000);
		si_fileWrite(&file_handle, SI_STR("random garbage"));

		curWriteTime = si_pathLastWriteTime(file_path);
		si_printf(
			"Has the file been changed?: %B (%lu difference)\n",
			lastWriteTime != curWriteTime,
			curWriteTime - lastWriteTime
		);

		si_fileClose(file_handle);
	}

	{
		siString str_hard = SI_STR("hardLink"),
				 str_soft = SI_STR("softLink"),
				 str_file = SI_STR("randomSiFile.silitxt");


		si_pathCreateHardLink(str_file, str_hard);
		si_pathCreateSoftLink(str_file, str_soft);

		si_pathRemove(str_file);
		si_pathRemove(str_hard);
		si_pathRemove(str_soft);

		si_printf("Temporary path of the system: %S\n", si_pathGetTmp());
	}
}

void example4(void) {
	si_printf("==============\n\n==============\nExample 4:\n");
	#define ROOT_PATH "Česnakaujančio-убийца-世界"

	/* Preparing a make-shift directory. */
	{
		si_pathCreateFolder(SI_STR(ROOT_PATH));

		si_pathCreateFolder(SI_STR(ROOT_PATH "/other"));
		siFile file = si_fileCreate(SI_STR(ROOT_PATH "/secret.txt"));
		si_fileWrite(&file, SI_STR(ROOT_PATH));
		si_fileClose(file);
		si_pathCreateHardLink(SI_STR(ROOT_PATH "/secret.txt"), SI_STR(ROOT_PATH "/hardLinkToSecret.link"));
	}

	siDirectory dir = si_directoryOpen(SI_STR(ROOT_PATH));
	siDirectoryEntry entry;

	usize count = 0;
	while (si_directoryPollEntryEx(&dir, &entry, false)) {
		si_printf(
			"%zu: %S ('%zu' bytes, type '%i')\n",
			count, entry.path, entry.path.len, entry.type
		);
		count += 1;
	}

	si_pathRemove(SI_STR(ROOT_PATH));
}

void example5(siAllocator* alloc) {
	si_printf("==============\n\n==============\nExample 5:\n");

	si_printf("Characters: %c %c\n", 'a', 65);
	si_printf("Decimals: %d %d %lu\n", 1977, 65000L, UINT64_MAX);
	si_printf("Preceding with blanks: %10d\n", 1977);
	si_printf("Preceding with zeros: %010d \n", 1977);
	si_printf("Some different radices: %d %x %o %#x %#o\n", 100, 100, 100, 100, 100);
	si_printf("Floats: %4.2f %+.0e %E %g\n", 3.1416, 3333333333333.1416, 3.1416, 1234.062400);
	si_printf("Width trick: %*d \n", 5, 10);
	si_printf("%.5s\n", "A string");
	si_printf("%B - %B (%#b, %#b)\n", true, false, true, false);
	si_printf("Pointer to the heap: %p\n", alloc);
	si_printf("This will print nothing: '%n', 100%%.\n", nil);
	si_printf(
		"%CThis text will be displayed in red%C, while this - %Cin blue%C!\n"
		"%CSome terminals might support 8-bit color%C, %Csome may even have 24-bit color support.%C\n",
		si_printColor3bit(siPrintColorAnsi_Red), si_printColor3bitEx(siPrintColorAnsi_Blue, true, true),
		si_printColor8bit(202), si_printColor24bit(90, 242, 166)
	);
	si_fprintf(
		si_stdout,
		"Unicode works both on Unix and Windows* (ąčęėįšųū„“)\n\t%C* - Works as long as the font supports the codepoint, which for some reason isn't common.%C\n",
		si_printColor3bit(siPrintColorAnsi_Yellow)
	);
}
