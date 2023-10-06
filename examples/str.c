#define SI_IMPLEMENTATION 1
#include <sili.h>


void example1(siAllocator* heap) {
    si_allocatorReset(heap);
    printf("==============\nExample 1:\n");

	siString str = si_stringMake(heap, "Labas, Pasauli!");
	printf("str: \"%s\"\n", str);

	si_stringAppend(&str, " Lithuanian, more like Russian amirite.");
	printf("str: \"%s\"\n", str);

	char front = si_stringFront(str);
	char back = si_stringBack(str);
	usize length = si_stringLen(str);
	printf("front: '%c', back: '%c', len: '%zd'\n", front, back, length);

	si_stringSet(&str, "Different sentence");
	printf("str: \"%s\"\n", str);

	si_stringPush(&str, '.');
	printf("str: \"%s\"\n", str);

	siString str2 = si_stringCopy(heap, str);
	b32 result = si_cstrEqual(str, str2);
	printf("(\"%s\" == \"%s\") returns a '%s' boolean\n", str, str2, (result ? "true" : "false"));

	isize pos = si_stringFind(str, "sentence");
	printf("The word 'sentence' was found at position '%zd' (Starting with the letter '%c')\n", pos, str[pos]);

	pos = si_stringFind(str, "random");
	printf("However, the word 'random' was not found, thus 'pos' equals to %zd\n", pos);

	si_stringReplace(&str, "Different", "Completely new");
	printf("str: \"%s\"\n", str);

	si_stringReplace(&str2, "Different", "The same");
	printf("str2: \"%s\"\n", str2);

	si_stringTrim(str, " sentence.");
	printf("str: \"%s\"\n", str);

	si_stringClear(str);
	printf("Length of str: '%zd'\n", si_stringLen(str));


	si_stringSet(&str2, "one.two.three.four.five");
	printf("Current str2: \"%s\"\n", str2);

	siArray(siString) list = si_stringSplit(heap, str2, ".");

	for_range (i, 0, si_arrayLen(list)) {
		printf("\tElement %zd: \"%s\"\n", i, list[i]);
	}
}

void example2(siAllocator* heap) {
    si_allocatorReset(heap);
    printf("==============\n\n==============\nExample 2:\n");
	/* Int stuff: */
	siString str = si_stringMake(heap, si_i64ToCstr(heap, -342));
	printf("str: \"%s\"\n", str);

	isize num = si_cstrToU64("9300");
	printf("num: %zd\n\n", num);


	/* Join, upper, lower, title, capitalize */
	si_stringSet(&str, "/home");
	printf("Original str: \"%s\"\n", str);

	si_stringJoin(&str, "random.txt", "/");
	printf("Joined str: \"%s\"\n", str);

	si_cstrUpper(str);
	printf("Upper str: \"%s\"\n", str);

	si_stringSet(&str, "I'VE COME TO MAKE AN ANNOUNCEMENT");
	printf("Original str: \"%s\"\n", str);

	si_cstrLower(str);
	printf("Lower str: \"%s\"\n", str);

	si_cstrTitle(str);
	printf("Titled str: \"%s\"\n", str);

	si_cstrCapitalize(str);
	printf("Capitalized str: \"%s\"\n", str);
}

void example3(siAllocator* heap) {
    si_allocatorReset(heap);
    printf("==============\n\n==============\nExample 3:\n");

	siString str = si_stringMake(heap, "\t       dnuora gniliart        ");
	printf("Before: '%s' (len: '%zd')\n", str, si_stringLen(str));

	si_stringStrip(str);
	printf("After: '%s' (len: '%zd')\n", str, si_stringLen(str));

	si_stringReverse(str);
	printf("'str' in reverse: '%s'\n", str);
}


int main(void) {
    siAllocator* heap = si_allocatorMake(SI_KILO(1));

	example1(heap);
	example2(heap);
	example3(heap);

	si_allocatorFree(heap);
	return 0;
}
