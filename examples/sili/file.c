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
	siArena aData = si_arenaMake(si_allocatorHeap(), SI_MEGA(1));
	siAllocator alloc = si_allocatorArena(&aData);

	example1(alloc);
	example2();
	example3();
	example4();
	example5(&alloc);

	si_arenaFree(&aData);
}


void example1(siAllocator alloc) {
	siArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator stack = si_allocatorArena(&aData);

	si_printf("==============\n\n==============\nExample 1:\n");
	siString file_random = SI_STR("random.txt");
	siString file_examples_file = SI_STR("examples/sili/file.c");
	siString unknown = SI_STRC("N/A");

	/* If the file doesn't exist or fails to open in any other way, then we will
	 * get an assertion error. */
	{
		siFile newFile = si_fileCreate(file_random);
		si_fileWriteStr(&newFile, SI_STR("A silly file\nwith three sili newlines\nbut not much else."));

		siString content = si_fileReadContents(newFile, alloc);
		content = si_stringReplaceAll(content, SI_STR("\n"), SI_STR("\\n"), alloc);

		si_printf(
			"About 'random.txt':\n\t"
				"Full path - '%S'\n\t"
				"Size - '%zu' bytes\n\t"
				"Content - '%S'\n\n",
			si_optionalGetOrDefault(si_pathGetFullName(file_random, stack), unknown),
			content.len, content
		);

		si_fileClose(&newFile);
	}

	{
		siFile file = si_fileOpen(file_examples_file);
		si_printfLn(
			"About 'examples/sili/file.c':\n\t"
				"Full path - '%S'\n\t"
				"Size - '%zu' bytes",
			si_optionalGetOrDefault(si_pathGetFullName(file_examples_file, stack), unknown),
			file.size
		);

		siArray(siString) lines = si_fileReadlines(file, alloc);
		si_printfLn(
			"Contents of '%S' ('%zd' lines in total):",
			si_pathBaseName(file_examples_file), lines.len
		);


		siString line;
		for_eachArrEx (line, i, lines) {
			si_printfLn("\tLine %zu (%zu bytes): '%S'", i + 1, line.len, line);
		}
		si_fileClose(&file);
	}
}

void example2(void)	{
	siArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator stack = si_allocatorArena(&aData);

	si_printLn("==============\n\n==============\nExample 2:");
	{
		siString str_random = SI_STR("random.txt"),
				 str_random2 = SI_STR("random-2.txt"),
				 str_renamed = SI_STR("renamed.txt");

		b32 exist = si_pathExists(str_random);
		if (!exist) {
			si_print("Since 'random.txt' doesn't exist, we'll just create one\n");

			siFile file = si_fileCreate(str_random);
			si_fileWriteStr(&file, SI_STR("QWERTYUIOP"));
			si_fileClose(&file);
		}

		siResult(isize) error = si_pathCopy(str_random, str_random2);
		si_printf(
			"Does 'random-2.txt' exist: %B (returned bytes: '%zi')\n\n",
			si_pathExists(str_random2), si_optionalGetOrDefault(error, -1)
		);

		siError res = si_pathMove(str_random, str_renamed); // 'si_pathRename' does the same thiing as well.
		si_printf(
			"Does 'random.txt' exist: %B\n"
			"'renamed.txt' outputs a %B (res: '%i')\n\n",
			si_pathExists(str_random), si_pathExists(str_renamed), res.code
		);

		res = si_pathRemove(str_random2);
		si_printfLn(
			"Does 'random-2.txt' exist: '%B' (res: '%i')",
			si_pathExists(str_random2), res.code
		);

		res = si_pathRemove(str_renamed);
		si_printfLn(
			"Does 'renamed.txt' exist: '%B' (res: '%i')",
			si_pathExists(str_renamed), res.code
		);
	}

	{
		siString path = SI_STR("example.c");
		siOption(siString) fullPath = si_pathGetFullName(path, stack);
		si_printfLn(
			"Information about '%S':\n\t"
				"Base name - '%S'\n\t"
				"Without extension - '%S'\n\t"
				"Extension - '%S'\n\t"
				"Full path - '%S'\n\t"
				"Is relative: %B",
			path, si_pathBaseName(path), si_pathWithoutExtension(path), si_pathExtension(path),
			si_optionalGetOrDefault(fullPath, SI_STR("NO_FULL_PATH_BECAUSE_ERROR")), si_pathIsRelative(path)
		);
	}
}

void example3(void)	{
	si_printLn("==============\n\n==============\nExample 3:");

	{
		siError res = si_pathRemove(SI_STR("SI_FILE_THAT_DOESNT_EXIST"));
#ifndef SI_NO_ERROR_STRUCT
		si_printfLn("Error '%S' occurred at \"%L\": '%S'", si_systemErrorName(res.code), res.location, si_systemErrorDesc(res.code));
#else
		si_printfLn("Error '%S' occurred: '%S'", si_systemErrorName(res.code), si_systemErrorDesc(res.code));
#endif
	}

	{
		siTime lastWriteTime, curWriteTime;
		siString file_path = SI_STR("randomSiFile.silitxt");

		siFile file_handle = si_fileCreate(file_path);

		lastWriteTime = si_fileLastWriteTime(file_handle);
		si_printfLn("Last write time: %lu", lastWriteTime);

		si_sleep(1000);
		si_fileWriteStr(&file_handle, SI_STR("random garbage"));

		curWriteTime = si_pathLastWriteTime(file_path);
		si_printfLn(
			"Has the file been changed?: %B (%li difference)",
			lastWriteTime != curWriteTime,
			curWriteTime - lastWriteTime
		);

		si_fileClose(&file_handle);
	}

	{
		siString str_hard = SI_STR("hardLink"),
				 str_soft = SI_STR("softLink"),
				 str_file = SI_STR("randomSiFile.silitxt");


		si_pathCreateHardLink(str_file, str_hard);
		si_pathCreateSoftLink(str_file, str_soft);

		si_pathRemove(str_soft);
		si_pathRemove(str_hard);
		si_pathRemove(str_file);

		si_printfLn("Temporary path of the system: %S", si_pathGetTmp());
	}
}

void example4(void) {
	si_printLn("==============\n\n==============\nExample 4:");
	#define ROOT_PATH "Česnakaujančio-убийца-世界"

	/* Preparing a make-shift directory. */
	{
		si_pathCreateFolder(SI_STR(ROOT_PATH));

		si_pathCreateFolder(SI_STR(ROOT_PATH "/other"));
		siFile file = si_fileCreate(SI_STR(ROOT_PATH "/secret.txt"));
		si_fileWriteStr(&file, SI_STR(ROOT_PATH));
		si_fileClose(&file);
		si_pathCreateHardLink(SI_STR(ROOT_PATH "/secret.txt"), SI_STR(ROOT_PATH "/hardLinkToSecret.link"));
		si_pathCreateHardLink(SI_STR(ROOT_PATH "/hardLinkToSecret.link"), SI_STR(ROOT_PATH "/softLinkToHardLink.link"));
	}

	siDirectory dir = si_directoryOpen(SI_STR(ROOT_PATH));
	siDirectoryIterator it;

	usize count = 0;
	while (si_directoryIterate(&dir, &it)) {
		si_printfLn(
			"%zu: %S ('%zu' bytes, type '%i')",
			count, it.path, it.path.len, it.type
		);
		count += 1;
	}

	si_pathRemove(SI_STR(ROOT_PATH));
}

void example5(siAllocator* alloc) {
	si_printLn("==============\n\n==============\nExample 5:\n");

	si_printfLn("Characters: %c %c", 'a', 65);
	si_printfLn("Decimals: %d %d %lu", 1977, 65000L, UINT64_MAX);
	si_printfLn("Preceding with blanks: %10d", 1977);
	si_printfLn("Preceding with zeros: %010d", 1977);
	si_printfLn("Some different radices: %d %x %o %#x %#o", 100, 100, 100, 100, 100);
	si_printfLn("Floats: %4.2f %+.0e %E %g", 3.1416, 3333333333333.1416, 3.1416, 1234.062400);
	si_printfLn("Width trick: %*d", 5, 10);
	si_printfLn("%.5s", "A string");
	si_printfLn("%B - %B (%#b, %#b)", true, false, true, false);
	si_printfLn("Pointer to the heap: %p", alloc);
	si_printfLn("This will print nothing: '%n', 100%%.", nil);
	si_printfLn(
		"%CThis text will be displayed in red%C, while this - %Cin blue%C!\n"
		"%CSome terminals might support 8-bit color%C, %Csome may even have 24-bit color support.%C",
		si_printColor3bit(siPrintColor3bit_Red), si_printColor3bitEx(siPrintColor3bit_Blue, true, true),
		si_printColor8bit(202), si_printColor24bit(90, 242, 166)
	);
	si_fprintfLn(
		si_stdout,
		SI_STR("Unicode works both on Unix and Windows* (ąčęėįšųū„“)\n\t%C* - Works as long as the font supports the codepoint, which for some reason isn't common.%C"),
		si_printColor3bit(siPrintColor3bit_Yellow)
	);
}
