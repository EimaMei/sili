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
	siArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)));
	siAllocator stack = si_allocatorArena(&aData);

	si_printfLn("==============\n\n==============\nExample 1:");
	siString file_random = SI_STR("random.txt");
	siString file_examples_file = SI_STR("examples/sili/file.c");

	/* If the file doesn't exist or fails to open in any other way, then we will
	 * get an assertion error. */
	{
		siFile newFile = si_fileCreate(file_random);
		si_fileWriteStr(newFile, SI_STR("A silly file\nwith three sili newlines\nbut not much else.\n"));

		siString content = si_fileReadContentsStr(newFile, alloc);
		content = si_stringReplaceAll(content, SI_STR("\n"), SI_STR("\\n"), alloc);

		si_printf(
			"About 'random.txt':\n\t"
				"Full path - '%s'\n\t"
				"Size - '%i' bytes\n\t"
				"Content - '%s'\n\n",
			si_pathGetFullName(file_random, stack), content.len, content
		);

		si_fileClose(newFile);
	}

	{
		siFile file = si_fileOpen(file_examples_file);
		si_printfLn(
			"About 'examples/sili/file.c':\n\t"
				"Full path - '%s'\n\t"
				"Size - '%i' bytes",
			si_pathGetFullName(file_examples_file, stack), si_fileSize(file)
		);

		siArray(siString) lines = si_fileReadlines(file, alloc);
		si_printfLn(
			"Contents of '%s' ('%i' lines in total):",
			si_pathBaseName(file_examples_file), lines.len
		);

		siString line;
		for_eachArrEx (line, i, lines) {
			si_printfLn("\tLine %2i (%3i bytes): '%s'", i + 1, line.len, line);
		}
		si_fileClose(file);
	}
}

void example2(void)	{
	siArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)));
	siAllocator stack = si_allocatorArena(&aData);

	si_printLn("==============\n\n==============\nExample 2:");
	{
		siString str_random = SI_STR("random.txt"),
				 str_random2 = SI_STR("random-2.txt"),
				 str_renamed = SI_STR("renamed.txt");

		bool exist = si_pathExists(str_random);
		if (!exist) {
			si_printLn("Since 'random.txt' doesn't exist, we'll just create one");

			siFile file = si_fileCreate(str_random);
			si_fileWriteStr(file, SI_STR("QWERTYUIOP"));
			si_fileClose(file);
		}

		isize error = si_pathCopy(str_random, str_random2);
		si_printfLn(
			"Does 'random-2.txt' exist: %t (returned bytes: '%i')",
			si_pathExists(str_random2), error
		);

		siSystemError res = si_pathMove(str_random, str_renamed); // 'si_pathRename' does the same thiing as well.
		si_printfLn(
			"Does 'random.txt' exist: %t\n"
			"'renamed.txt' outputs a %t (%s)",
			si_pathExists(str_random), si_pathExists(str_renamed), si_systemErrorName(res)
		);

		res = si_pathRemove(str_random2);
		si_printfLn(
			"Does 'random-2.txt' exist: '%t' (%s)",
			si_pathExists(str_random2), si_systemErrorName(res)
		);

		res = si_pathRemove(str_renamed);
		si_printfLn(
			"Does 'renamed.txt' exist: '%t', ",
			si_pathExists(str_renamed), si_systemErrorName(res)
		);
	}

	{
		si_pathCreateFolder(SI_STR("some_si_folder"));
		siString path = SI_STR("some_si_folder/example.tar.gz");
	
		siFile file = si_fileCreate(path);
		si_fileClose(file);

		si_printfLn(
			"\nInformation about '%s':\n\t"
				"Base name - '%s'\n\t"
				"Full path - '%s'\n\t"
				"Stem - '%s'\n\t"
				"Short stem - '%s'\n\t"
				"Extension - '%s'\n\t"
				"Long extension - '%s'\n\t"
				"Is relative: %t",
			path,
			si_pathBaseName(path),  si_pathGetFullName(path, stack),
			si_pathStem(path), si_pathShortStem(path),
			si_pathExtension(path), si_pathLongExtension(path),
			si_pathIsRelative(path)
		);

		si_pathRemove(SI_STR("some_si_folder"));
	}
}

void example3(void)	{
	si_printLn("==============\n\n==============\nExample 3:");

	{
		siSystemError res = si_pathRemove(SI_STR("SI_FILE_THAT_DOESNT_EXIST"));
		si_printfLn("Error '%s' occurred: '%s'", si_systemErrorName(res), si_systemErrorDesc(res));
	}

	{
		siTime lastWriteTime, curWriteTime;
		siString file_path = SI_STR("randomSiFile.silitxt");

		siFile file_handle = si_fileCreate(file_path);

		lastWriteTime = si_fileLastWriteTime(file_handle);
		si_printfLn(
			"Last write time: %s - %i",
			si_timeToString(
				si_timeToCalendar(lastWriteTime), SI_STR("yyyy-MM-dd hh:mm:ss"), SI_ARR_STACK(64)
			), 
			lastWriteTime
		);

		si_sleep(SI_TIME_SEC(1));
		si_fileWriteStr(file_handle, SI_STR("random garbage"));

		curWriteTime = si_pathLastWriteTime(file_path);
		si_printfLn(
			"Last write time: %s - %i\n"
			"Has the file been changed?: %t (%i difference)",
			si_timeToString(
				si_timeToCalendar(curWriteTime), SI_STR("yyyy-MM-dd hh:mm:ss"), SI_ARR_STACK(64)
			), 
			curWriteTime, 
			lastWriteTime != curWriteTime, curWriteTime - lastWriteTime
		);

		si_fileClose(file_handle);
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

		si_printfLn("Temporary path of the system: %s", si_pathGetTmp());
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
		si_fileWriteStr(file, SI_STR(ROOT_PATH));
		si_fileClose(file);
		si_pathCreateHardLink(SI_STR(ROOT_PATH "/secret.txt"), SI_STR(ROOT_PATH "/hardLinkToSecret"));
		si_pathCreateSoftLink(SI_STR(ROOT_PATH "/secret.txt"), SI_STR(ROOT_PATH "/softLinkToSecret"));
	}

	siDirectory dir = si_directoryOpen(SI_STR(ROOT_PATH));
	siDirectoryIterator it;

	usize count = 0;
	while (si_directoryIterate(&dir, &it)) {
		si_printfLn(
			"%i: %s ('%i' bytes, type '%i')",
			count, it.path, it.path.len, it.type
		);
		count += 1;
	}

	si_pathRemove(SI_STR(ROOT_PATH));
}

void example5(siAllocator* alloc) {
	si_printLn("==============\n\n==============\nExample 5:\n");

	si_printfLn("Characters: %c %c", 'a', 65);
	si_printfLn("Decimals: %d %d %i", 1977, 65000L, UINT64_MAX);
	si_printfLn("Preceding with blanks: %10d", 1977);
	si_printfLn("Preceding with  zeros: %010d", 1977);
	si_printfLn("Some different radices: %d %x %o %#x %#o", 100, 100, 100, 100, 100);
	si_printfLn("Floats: %4.2f %+.0e %E %g", 3.1416, 3333333333333.1416, 3.1416, 1234.062400);
	si_printfLn("Width trick: %*d", 5, 10);
	si_printfLn("%.5s", SI_STR("A string"));
	si_printfLn("%t - %t (%#b, %#b)", true, false, true, false);
	si_printfLn("Pointer to the heap: %p", alloc);
	si_printfLn(
		"%CThis text will be displayed in red%C, while this - %Cin blue%C!\n"
		"%CSome terminals might support 8-bit color%C, %Csome may even have 24-bit color support.%C",
		si_printColor3bit(siPrintColor3bit_Red), si_printColor3bit(siPrintColor3bit_Blue, siPrintColorAnsiBits_Bold | siPrintColorAnsiBits_Light),
		si_printColor8bit(202), si_printColor24bit(90, 242, 166)
	);
	si_fprintfLn(
		si_stdout,
		SI_STR("Unicode works both on Unix and Windows* (ąčęėįšųū„“)\n\t%C* - Works as long as the font supports the codepoint, which for some reason isn't common.%C"),
		si_printColor3bit(siPrintColor3bit_Yellow)
	);
}
