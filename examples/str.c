#define SI_IMPLEMENTATION 1
#include <sili.h>


void example1(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\nExample 1:\n");

	siString str = si_stringMake(heap, "Labas, Pasauli!");
	si_printf("str: \"%s\"\n", str);

	si_stringAppend(&str, " Lithuanian, more like Russian amirite.");
	si_printf("str: \"%s\"\n", str);

	char front = si_stringFront(str);
	char back = si_stringBack(str);
	usize length = si_stringLen(str);
	si_printf("front: '%c', back: '%c', len: '%zd'\n", front, back, length);

	si_stringSet(&str, "Different sentence");
	si_printf("str: \"%s\"\n", str);

	si_stringPush(&str, '.');
	si_printf("str: \"%s\"\n", str);

	siString str2 = si_stringCopy(heap, str);
	b32 result = si_cstrEqual(str, str2);
	si_printf("(\"%s\" == \"%s\") returns a '%B' boolean\n", str, str2, result);

	isize pos = si_stringFind(str, "sentence");
	si_printf("The word 'sentence' was found at position '%zd' (Starting with the letter '%c')\n", pos, str[pos]);

	pos = si_stringFind(str, "random");
	si_printf("However, the word 'random' was not found, thus 'pos' equals to %zd\n", pos);

	si_stringReplace(&str, "Different", "Completely new");
	si_printf("str: \"%s\"\n", str);

	si_stringReplace(&str2, "Different", "The same");
	si_printf("str2: \"%s\"\n", str2);

	si_stringTrim(str, " sentence.");
	si_printf("str: \"%s\"\n", str);

	si_stringClear(str);
	si_printf("Length of str: '%zd'\n", si_stringLen(str));

	si_stringSet(&str2, "one.two.three.four.five");
	si_printf("Current str2: \"%s\"\n", str2);

	siArray(siString) list = si_stringSplit(str2, heap, ".");

	for_range (i, 0, si_arrayLen(list)) {
		si_printf("\tElement %zd: \"%s\"\n", i, list[i]);
	}
}

void example2(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\n\n==============\nExample 2:\n");

	siString str;
	
	{
		siAllocator* stack = si_allocatorMake(32);
		usize len;

		char* num = si_i64ToCstr(stack, -342, &len);
		str = si_stringMakeLen(heap, num, len);
		si_printf("str: \"%s\"\n", str);
	}

	{
		isize num = si_cstrToU64("9300");
		si_printf("num: %zd\n\n", num);

		char buf[1024];
		usize len;
		siAllocator alloc = si_allocatorMakeTmp(buf, sizeof(buf));
		
		char* str = si_f64ToCstr(&alloc, FLOAT32_MAX, &len);
		si_printf("%.*s\n", len, str);
	}

	{
		si_stringSet(&str, "/home");
		si_printf("Original str: \"%s\"\n", str);

		si_stringJoin(&str, "random.txt", "/");
		si_printf("Joined str: \"%s\"\n", str);

		si_cstrUpper(str);
		si_printf("Upper str: \"%s\"\n", str);

		si_stringSet(&str, "I'VE COME TO MAKE AN ANNOUNCEMENT");
		si_printf("Original str: \"%s\"\n", str);

		si_cstrLower(str);
		si_printf("Lower str: \"%s\"\n", str);

		si_cstrTitle(str);
		si_printf("Titled str: \"%s\"\n", str);

		si_cstrCapitalize(str);
		si_printf("Capitalized str: \"%s\"\n", str);
	}
}

void example3(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\n\n==============\nExample 3:\n");

	siString str = si_stringMake(heap, "\t       dnuora gniliart        ");
	si_printf("Before: '%s' (len: '%zd')\n", str, si_stringLen(str));

	si_stringStrip(str);
	si_printf("After: '%s' (len: '%zd')\n", str, si_stringLen(str));

	si_stringReverse(str);
	si_printf("'str' in reverse: '%s'\n", str);
}


int main(void) {
	siAllocator* heap = si_allocatorMake(SI_KILO(1));

	example1(heap);
	example2(heap);
	example3(heap);

	si_allocatorFree(heap);
	return 0;
}
