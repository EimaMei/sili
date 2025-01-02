#define SI_IMPLEMENTATION 1
#include <sili.h>


/* Shows the primary functions for making, reading and manipulating string data. */
void example1(siAllocator alloc);
/* Shows the secondary functions for converting and manipulating strings. */
void example2(siAllocator alloc);


int main(void) {
	siAllocatorArena aData = si_arenaMake(si_allocatorHeap(), SI_KILO(1));
	siAllocator alloc = si_allocatorArena(&aData);

	example1(alloc);
	example2(alloc);

	si_arenaFree(&aData);
}


void example1(siAllocator alloc) {
	si_printf("==============\nExample 1:\n");

	si_print("Scope 1:\n");
	{
		siString strStatic = SI_STR("Hello, world!");
		si_printf("\tstr: \"%S\" or \"%.*s\"\n", strStatic, strStatic.len, strStatic.data);

		siString str = si_stringCopy(strStatic, alloc);
		si_printf("\t(str == strStatic) returns a '%B' boolean\n", si_stringEqual(str, strStatic));

		b32 allocated = si_stringAppend(&str, SI_STR(" Labas, pasauli! Ciao, mondo!"));
		si_printf("\tstr: \"%S\" (Was allocated: %B)\n", str, allocated);
	}

	si_print("Scope 2:\n");
	{
		siString str = si_stringMake("Dynamically allocated string", alloc);
		si_printf("\tstr: '%S', len: '%zi', capacity: '%zi'\n", str, str.len, str.capacity);

		i32 front = si_stringAtFront(str);
		i32 back = si_stringAtBack(str);
		si_printf("\tfront: '%c', back: '%c'\n", front, back);

		b32 allocated = si_stringSet(&str, SI_STR("A different string"));
		si_printf("\tstr: \"%S\", allocated: '%B', len: '%zi', capacity: '%zi'\n", str, allocated, str.len, str.capacity);

		allocated = si_stringPush(&str, '.');
		si_printf("\tstr: \"%S\", allocated: '%B', len: '%zi'\n", str, allocated, str.len);
	}

	si_print("Scope 3:\n");
	{
		siString str = si_stringMake(
			"Geri vyrai geroj girioj gerą girą gėrė ir gerdami gyrė: "
			"geriems vyrams geroj girioj gerą girą gera gert.",
			alloc
		);
		si_printf("\tstr: '%S', len: '%zi', capacity: '%zi'\n", str, str.len, str.capacity);
		siString str_ger = SI_STR("ger");

		isize posFirst = si_stringFind(str, str_ger),
			  posLast = si_stringFindLast(str, str_ger);
		isize occurences = si_stringFindCount(str, str_ger);

		si_printf(
			"\tThe substring '%S' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.\n",
			str_ger, posFirst, posLast, occurences
		);

		isize pos = si_stringFind(str, SI_STR("Žąsys"));
		si_printf("\tHowever, the substring 'Žąsys' wasn't found and so, the function returns '%zi'\n", pos);
	}

	si_print("Scope 4:\n");
	{
		siString str = si_stringMake("smaug giganteus", alloc);
		si_printf("\tstr: '%S', len: '%zi', capacity: '%zi'\n", str, str.len, str.capacity);

		si_stringTrim(&str, SI_STR("s"));
		si_printf("\tstr: '%S', len: '%zi', capacity: '%zi'\n", str, str.len, str.capacity);

		b32 allocated = si_stringInsert(&str, SI_STR("the "), countof_str("maug "));
		si_printf("\tstr: '%S', allocated: '%B', len: '%zi', capacity: '%zi'\n", str, allocated, str.len, str.capacity);

		si_stringErase(&str, 3, 6);
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

	}

	si_print("Scope 5:\n");
	{
		siString str = si_stringMake("one.two.three.four.five", alloc);
		si_printf("\tstr: '%S', len: '%zi', capacity: '%zi'\n", str, str.len, str.capacity);

		siArray(siString) list = si_stringSplit(str, SI_STR("."), alloc);

		si_print("\tElements: ");
		for_eachArr (siString, subStr, list) { si_printf("\"%S\" ", *subStr); }
		si_print("\n");

		si_stringClear(&str);
		si_printf("\tLength of str: '%zi'\n", str.len);
	}

	si_print("Scope 6:\n");
	{
		siString str = si_stringMake("\t       dnuora gniliart        ", alloc);
		si_printf("Before: '%S' (len: '%zi')\n", str, str.len);

		si_stringStrip(&str);
		si_printf("After: '%S' (len: '%zi')\n", str, str.len);

		si_stringReverse(str);
		si_printf("'str' in reverse: '%S'\n", str);
	}
}

void example2(siAllocator alloc) {
	si_printf("==============\n\n==============\nExample 2:\n");
	siBuffer arr = SI_BUF(siString, SI_STR("/home"), SI_STR("user"), SI_CSTR("Desktop"), SI_STR("RANDOM.txt"));

	siString str = si_stringFromInt(-342, alloc);
	si_printf("str: \"%S\"\n", str);

	i64 num = si_stringToInt(SI_STR("  9300  "));
	si_printf("num: %li\n", num);

	str = si_stringFromFloat(FLOAT32_MAX, alloc);
	si_printf("str: %S\n", str);

	str = si_stringJoin(arr, SI_STR("/"), alloc);
	si_printf("Joined str: \"%S\"\n", str);

	si_stringUpper(str);
	si_printf("Upper str: \"%S\"\n", str);

	si_stringLower(str);
	si_printf("Lower str: \"%S\"\n", str);

	str = si_stringMake("something about a fox and it jumping", alloc);
	si_stringTitle(str);
	si_printf("Titled str: \"%S\"\n", str);

	si_stringCapitalize(str);
	si_printf("Capitalized str: \"%S\"\n", str);
}
