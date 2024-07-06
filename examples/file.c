#define SI_IMPLEMENTATION 1
#include <sili.h>


void example1(siAllocator* heap) {
	siAllocator stack = si_allocatorMakeStack(SI_KILO(4));

	si_printf("==============\n\n==============\nExample 1:\n");

	/* If the file doesn't exist or fails to open any other way, then we will
	 * get an assertion error. */
	siFile newFile = si_fileCreate("random.txt");
	si_fileWrite(&newFile, "A silly file\nwith three sili newlines\nbut not much else.");

	siString content = si_fileReadContents(newFile, heap);
	si_stringReplaceAll(&content, SI_STR("\n"), SI_STR("\\n"), heap);

	si_printf(
			"About 'random.txt':\n\t"
			"Full path - '%S'\n\t"
			"Size - '%zu' bytes\n\t"
			"Content - '%S'\n\n",
		si_pathGetFullName(SI_STR("random.txt"), &stack), content.len, content
	);


	siFile file = si_fileOpen("examples/file.c");
	si_printf(
		"About 'examples/file.c':\n\t"
			"Full path - '%S'\n\t"
			"Size - '%zu' bytes\n",
		si_pathGetFullName(SI_STR("examples/file.c"), &stack), file.size
	);

	siArray(siString) fileLine = si_fileReadlines(file, heap);
#if 0
	si_printf(
		"Contents of '%S' ('%zd' lines in total):\n",
		si_pathBaseName(), si_arrayLen(fileLine)
	);
#endif

	for_range (i, 0, si_arrayLen(fileLine)) {
		si_stringStrip(&fileLine[i]);
		si_printf("\tLine %zu: '%S'\n", i + 1, fileLine[i]);
	}
	si_fileClose(file);
	si_allocatorReset(heap);

	si_fileWriteAtLine(&newFile, SI_STR("and now we have a changed line"), 1);
	siArray(siString) newFileLines = si_fileReadlines(newFile, heap);
	si_printf(
		"Contents of '%S' ('%zd' lines in total):\n",
		si_pathBaseName("randomDir/random.txt"), si_arrayLen(newFileLines)
	);

	for_range (i, 0, si_arrayLen(newFileLines)) {
		si_stringStrip(&newFileLines[i]);
		si_printf("\tLine %zu: '%S'\n", i + 1, newFileLines[i]);
	}
	si_fileClose(newFile);
}

#if 0
void example2(void)	{
	siAllocator* stack = si_allocatorMake(4096);
	si_printf("==============\n\n==============\nExample 2:\n");

	b32 exist = si_pathExists("example.c");

	exist = si_pathExists("random.txt");
	if (!exist) {
		SI_PANIC();
		si_printf("Since 'random.txt' doesn't exist, we'll just create one\n");

		siFile file = si_fileCreate("random.txt");
		si_fileWrite(&file, "QWERTYUIOP");
		si_fileClose(file);
	}

	isize res = si_pathCopy("random.txt", "random-2.txt");
	si_printf(
		"Does 'random-2.txt' exist: %B (returned bytes: '%zi')\n",
		si_pathExists("random-2.txt"), res
	);

	res = si_pathMove("random.txt", "renamed.txt");
	si_printf(
		"Does 'random.txt' exist: %B\n'renamed.txt' outputs a %B (res: '%zi')\n",
		si_pathExists("random.txt"), si_pathExists("renamed.txt"), res
	);

	cstring path = "example.c";
	siString fullPath = si_pathGetFullName(stack, path);
	si_printf(
		"Information about '%S':\n\t"
			"Base name - '%S'\n\t"
			"Extension - '%S'\n\t"
			"Full path - '%S'\n\t"
			"Is relative: %B\n",
		path, si_pathBaseName(path), si_pathExtension(path),
		fullPath, si_pathIsRelative(path)
	);

	res = si_pathRemove("random-2.txt");
	si_printf(
		"Does 'random-2.txt' exist: '%B' (res: '%B')\n",
		si_pathExists("random-2.txt"), res
	);

	res = si_pathRemove("renamed.txt");
	si_printf(
		"Does 'renamed.txt' exist: '%B' (res: '%B')\n",
		si_pathExists("renamed.txt"), res
	);
}

void example3(void)	{
	si_printf("==============\n\n==============\nExample 3:\n");
	{
		si_pathRemove("SI_FILE_THAT_DOESNT_EXIST");

		b32 res = si_pathCreateFolder("testFolder");
		SI_ASSERT(res);

		siFilePermissions perms = si_pathPermissions("testFolder");
		si_printf("Permissions of 'testFolder' (in octal): %o\n", perms);

		si_pathEditPermissions("testFolder", SI_FS_PERM_ALL);
		perms = si_pathPermissions("testFolder");
		si_printf("Permissions of 'testFolder' (in octal): %o\n", perms);

		si_pathRemove("testFolder");
	}

	{
		siFile file = si_fileCreate("randomSiFile.txt");

		u64 lastWriteTime = file.lastWriteTime;
		u64 curWriteTime = si_pathLastWriteTime(file.filename);

		si_sleep(1000);
		si_printf("Has the file been changed?: %B\n", lastWriteTime != curWriteTime);

		si_fileWrite(&file, "random garbage");
		curWriteTime = si_pathLastWriteTime(file.filename);
		si_printf("Has the file been changed?: %B\n", lastWriteTime != curWriteTime);

		si_pathCreateHardLink(file.filename, "hardLink");
		si_pathCreateSoftLink(file.filename, "softLink");
		si_fileClose(file);

		si_pathRemove(file.filename);
		si_pathRemove("hardLink");
		si_pathRemove("softLink");

		si_printf("Temporary path of the system: %S\n", si_pathGetTmp());
	}
}

void example4(siAllocator* alloc) {
	si_allocatorReset(alloc);
	si_printf("==============\n\n==============\nExample 4:\n");

	#define ROOT_PATH "Česnakaujančio-убийца-世界"
	si_pathCreateFolder(ROOT_PATH);

	si_pathCreateFolder(ROOT_PATH "/other");
	siFile file = si_fileCreate(ROOT_PATH "/secret.txt");
	si_fileWrite(&file, ROOT_PATH);
	si_fileClose(file);
	si_pathCreateHardLink(ROOT_PATH "/secret.txt", ROOT_PATH "/hardLinkToSecret.link");

	siDirectory dir = si_dirOpen(alloc, ROOT_PATH);
	siDirectoryEntry entry;

	usize count = 0;
	while (si_dirPollEntry(dir, &entry)) {
		si_printf("%zu: %S ('%zu' bytes, '%i' type)\n", count, entry.path, entry.len, entry.type);
		count += 1;
	}

	si_pathRemove(ROOT_PATH);
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
	si_printf("%CRThis text will be displayed in red%C, while this: %CBin blue%C!\n");
	si_fprintf(SI_STDOUT, "Unicode works both on Unix and Windows* (ąčęėįšųū„“)\n\t%CY* - Works as long as the font supports the codepoint, which for some reason isn't common.%C\n");
}
#endif

int main(void) {
	siAllocator heap = si_allocatorMake(SI_MEGA(1));

	example1(&heap);
	//example2();
	//example3();
	//example4(heap);
	//example5(heap);

	si_allocatorFree(&heap);
	return 0;
}
