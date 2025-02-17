#define SI_IMPLEMENTATION 1
#include <sili.h>


/* Shows the primary functions for making, reading and manipulating string data. */
void example1(siAllocator alloc);
/* Shows the secondary functions for converting and manipulating strings. */
void example2(siAllocator alloc);

int main(void) {
	siAllocatorArena aData = si_arenaMake(si_allocatorHeap(), SI_KILO(1));
	siAllocator alloc = si_allocatorArena(&aData);

	//example1(alloc);
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

	}

	si_print("Scope 2:\n");
	{
		siBuilder b = si_builderMake(256, alloc);
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		si_builderWriteStr(&b, SI_STR("Dynamically allocated string"));
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		si_builderWriteByte(&b, '.');
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		si_builderWriteRune(&b, 0x00000439); // UTF-32 'й'
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		si_builderWriteStrQuoted(&b, SI_STR("Hello world."));
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		si_builderWriteStrQuotedRune(&b, SI_STR("Labas, pasauli!"), 0x0000201E, 0x0000201C); // '„' and '“' characters
		si_printf("\tstr: '%S', len: '%zi'\n", b.len, b.data, b.len);

		siString str = si_builderToStr(b);
		i32 front = si_stringAtFront(str);
		i32 back = si_stringAtBack(str);
		si_printf("\tfront: '%lc', back: '%lc'\n", front, back);

	}

	si_print("Scope 3:\n");
	{
		siString str = SI_STR("Geri vyrai geroj girioj gerą girą gėrė ir gerdami gyrė: geriems vyrams geroj girioj gerą girą gera gert.");
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

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
		siString str = SI_STR("smaug giganteus");
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

		str = si_stringTrim(str, SI_STR("s"));
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

		str = si_stringInsert(str, SI_STR("the "), countof_str("maug "), alloc);
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

		str = si_stringRemoveAll(str, SI_STR("gigant"), alloc);
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

	}

	si_print("Scope 5:\n");
	{
		siString str = SI_STR("one.two.three.four.five");
		si_printf("\tstr: '%S', len: '%zi'\n", str, str.len);

		siBuffer(siString) list = si_stringSplit(str, SI_STR("."), alloc);

		si_printf("\tElements: %S\n", si_stringFromBuffer(list, "%S", SI_BUF_STACK(64)));
	}

	si_print("Scope 6:\n");
	{
		siString str = SI_STR("\t       dnuora gniliart        ");
		si_printf("\tBefore: '%S' (len: '%zi')\n", str, str.len);

		str = si_stringStrip(str);
		si_printf("\tAfter: '%S' (len: '%zi')\n", str, str.len);

		siString reverse = si_stringReverse(str, alloc);
		si_printf("\t'str' in reverse: '%S'\n", reverse);
	}
}

void example2(siAllocator alloc) {
	si_printf("==============\n\n==============\nExample 2:\n");

	siString str;
	{
		siBuffer(u8) buf = SI_BUF_ALLOC(u8, 4, alloc);
		str = si_stringFromInt(-342, buf);
		si_printf("str: \"%S\"\n", str);

		i64 num = si_stringToInt(SI_STR("  9300  "));
		si_printf("num: %li\n", num);

		str = si_stringFromFloat(FLOAT32_MAX, SI_BUF_ALLOC(u8, 128, alloc));
		si_printf("str: %S\n", str);

		siBuffer(siString) arr = SI_BUF(siString, SI_STR("/home"), SI_STR("user"), SI_STR("Desktop"), SI_STR("RANDOM-ąčęėįšųū-òàèéç-йцукенвыамсч.txt"));
		str = si_stringJoin(arr, SI_STR("/"), alloc);
		si_printf("Joined str: \"%S\"\n", str);
	}


	str = si_stringUpper(str, alloc);
	si_printf("Upper str: \"%S\"\n", str);

	str = si_stringLower(str, alloc);
	si_printf("Lower str: \"%S\"\n", str);
}
