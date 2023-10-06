#define SI_IMPLEMENTATION 1
#include <sili.h>


void example1(siAllocator* heap) {
    siAllocator* stack = si_allocatorMakeStack(4096);
	printf("==============\n\n==============\nExample 1:\n");

	siFile file = si_fileOpen("examples/file.c"); /* If the file doesn't exist or fails to open any other way, then we will get an assertion error. */
	printf(
        "About 'examples/file.c':\n\t"
            "Full path - '%s'\n\t"
            "Size - '%zu' bytes\n",
        si_pathGetFullName(stack, "examples/file.c"), file.size
    );

	siFile newFile = si_fileCreate("random.txt");
	si_fileWrite(&newFile, "A silly file\nwith a sili newline.");
    siString content = si_fileRead(heap, newFile);

    printf(
        "About 'random.txt':\n\t"
            "Full path - '%s'\n\t"
            "Size - '%zu' bytes\n\t"
            "Content - '%s'\n\n",
        si_pathGetFullName(stack, "random.txt"), newFile.size,
        content
    );

	siArray(siString) fileLine = si_fileReadlines(heap, file);
	printf(
        "Contents of '%s' ('%zd' lines in total):\n",
        si_pathBaseName("file.c"), si_arrayLen(fileLine)
    );

	for_range (i, 0, si_arrayLen(fileLine)) {
		si_stringStrip(fileLine[i]);
		printf("\tLine %zd: '%s'\n", i, fileLine[i]);
	}
    si_fileClose(file);
    si_allocatorReset(heap);

	si_fileWriteAtLine(&newFile, "but now we have a changed line", 1);
	siArray(siString) newFileLines = si_fileReadlines(heap, newFile);
    printf(
        "Contents of '%s' ('%zd' lines in total):\n",
        si_pathBaseName("exmp/random.txt"), si_arrayLen(newFileLines)
    );

	for_range (i, 0, si_arrayLen(newFileLines)) {
		si_stringStrip(newFileLines[i]);
		printf("\tLine %zd: '%s'\n", i, newFileLines[i]);
	}
	si_fileClose(newFile);
}

void example2(void)	{
    siAllocator* stack = si_allocatorMake(4096);
	printf("==============\n\n==============\nExample 2:\n");

	b32 exist = si_pathExists("example.c");
	printf("File 'example.c' %s\n", (exist ? "DOES exist" : "DOESN'T exist"));

	exist = si_pathExists("random.txt");
	if (!exist) {
		printf("Since 'random.txt' doesn't exist, we'll just create one\n");

		siFile file = si_fileCreate("random.txt");
		si_fileWrite(&file, "KANT RUINED US ALL");
		si_fileClose(file);
	}

	b32 res = si_pathCopy("random.txt", "random-2.txt");
	printf(
        "Does 'random-2.txt' exist: '%u' (res: '%u')\n",
        si_pathExists("random-2.txt"), res
    );

	res = si_pathMove("random.txt", "renamed.txt");
	printf(
        "Does 'random.txt' exist:'%u'\n'renamed.txt' outputs a '%u' (res: '%u')\n",
        si_pathExists("random.txt"), si_pathExists("renamed.txt"), res
    );

    cstring path = "example.c";
	siString fullPath = si_pathGetFullName(stack, path);
	printf(
        "Information about '%s':\n\t"
            "Base name - '%s'\n\t"
            "Extension - '%s'\n\t"
            "Full path - '%s'\n\t"
            "Is relative: %u\n",
        path, si_pathBaseName(path), si_pathExtension(path),
        fullPath, si_pathIsRelative(path)
    );

	res = si_pathRemove("random-2.txt");
	printf(
        "Does 'random-2.txt' exist: '%u' (res: '%u')\n",
        si_pathExists("random-2.txt"), res
    );

	res = si_pathRemove("renamed.txt");
	printf(
        "Does 'renamed.txt' exist: '%u' (res: '%u')\n",
        si_pathExists("renamed.txt"), res
    );
}


int main(void) {
    siAllocator* heap = si_allocatorMake(SI_KILO(8));

    example1(heap);
    example2();

    si_allocatorFree(heap);
	return 0;
}
