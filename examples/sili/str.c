#define SI_IMPLEMENTATION 1
#include <sili.h>


/* Shows the primary functions for making, reading and manipulating string data. */
void example1(siAllocator alloc);
/* Shows the secondary functions for converting and manipulating strings. */
void example2(siAllocator alloc);

int main(void) {
	siArena aData = si_arenaMake(si_allocatorHeap(), SI_KILO(1));
	siAllocator alloc = si_allocatorArena(&aData);

	example1(alloc);
	example2(alloc);

	si_arenaFree(&aData);
}

/* Prints the specified integer map. */
void print_map(siString comment, siMap(i32) map);

void example1(siAllocator alloc) {
	si_printLn("==============\nExample 1:");

	si_printLn("Scope 1:");
	{
		siString strStatic = SI_STR("Hello, world!");
		si_printfLn("\tstr: \"%S\" or \"%.*s\"", strStatic, strStatic.len, strStatic.data);

		siString str = si_stringCopy(strStatic, alloc);
		si_printfLn("\t(str == strStatic) returns a '%B' boolean", si_stringEqual(str, strStatic));

	}

	si_printfLn("Scope 2:");
	{
		siBuilder b = si_builderMake(256, alloc);
		si_printfLn("\tcapacity: '%zi' len: '%zi' grow: '%zi'", b.capacity, b.len, b.grow);

		si_builderWriteStr(&b, SI_STR("Dynamically allocated string"));
		si_printfLn("\tstr: '%.*s', len: '%zi'", b.len, b.data, b.len);

		si_builderWriteByte(&b, '.');
		si_printfLn("\tstr: '%.*s', len: '%zi'", b.len, b.data, b.len);

		si_builderWriteRune(&b, 0x00000439); // UTF-32 'й'
		si_printfLn("\tstr: '%.*s', len: '%zi'", b.len, b.data, b.len);

		si_builderWriteStrQuoted(&b, SI_STR("Hello world."));
		si_printfLn("\tstr: '%.*s', len: '%zi'", b.len, b.data, b.len);

		si_builderWriteStrQuotedRune(&b, SI_STR("Labas, pasauli!"), 0x0000201E, 0x0000201C); // '„' and '“' characters
		si_printfLn("\tstr: '%.*s', len: '%zi'", b.len, b.data, b.len);

		siString str = si_builderToStr(b);
		i32 front = si_stringAtFront(str);
		i32 back = si_stringAtBack(str);
		si_printfLn("\tfront: '%lc', back: '%lc'", front, back);
	}

	si_printfLn("Scope 3:");
	{
		siString str = SI_STR("Geri vyrai geroj girioj gerą girą gėrė ir gerdami gyrė: geriems vyrams geroj girioj gerą girą gera gert.");
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

		siString str_ger = SI_STR("ger");
		isize posFirst = si_stringFind(str, str_ger),
			  posLast = si_stringFindLast(str, str_ger);
		isize occurences = si_stringFindCount(str, str_ger);

		si_printfLn(
			"\tThe substring '%S' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.",
			str_ger, posFirst, posLast, occurences
		);

		isize pos = si_stringFind(str, SI_STR("Žąsys"));
		si_printfLn("\tHowever, the substring 'Žąsys' wasn't found and so, the function returns '%zi'", pos);
	}

	si_printfLn("Scope 4:");
	{
		siString str = SI_STR("smaug giganteus");
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

		str = si_stringTrim(str, SI_STR("s"));
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

		str = si_stringInsert(str, SI_STR("the "), countof_str("maug "), alloc);
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

		str = si_stringRemoveAll(str, SI_STR("gigant"), alloc);
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

	}

	si_printfLn("Scope 5:");
	{
		siString str = SI_STR("one.two.three.four.five");
		si_printfLn("\tstr: '%S', len: '%zi'", str, str.len);

		siArray(siString) list = si_stringSplit(str, SI_STR("."), alloc);
		si_printfLn("\tElements: %S", si_stringFromArray(list, "%S", SI_ARR_STACK(64)));
	}

	si_printfLn("Scope 6:");
	{
		siString str = SI_STR("\t       dnuora gniliart        ");
		si_printfLn("\tBefore: '%S' (len: '%zi')", str, str.len);

		str = si_stringStrip(str);
		si_printfLn("\tAfter: '%S' (len: '%zi')", str, str.len);

		siString reverse = si_stringReverse(str, alloc);
		si_printfLn("\t'str' in reverse: '%S'", reverse);
	}
}


void print_map(siString comment, siMap(i32) map) {
	si_printfStr(comment);

	siString key;
	i32 value;
    for_eachMapEx (key, value, map) {
		si_printf("[%S] = %i; ", key, value);
	}
	si_print("\n");
}

void example2(siAllocator alloc) {
	si_printfLn("==============\n\n==============\nExample 2:\n");

	siString str;
	{
		siArray(u8) buf = si_arrayMakeReserve(u8, 4, alloc);
		str = si_stringFromInt(-342, buf);
		si_printfLn("str: \"%S\"", str);

		i64 num = si_stringToInt(SI_STR("  9300  "));
		si_printfLn("num: %li", num);

		str = si_stringFromFloat(FLOAT32_MAX, si_arrayMakeReserve(u8, 128, alloc));
		si_printfLn("str: %S", str);

		siArray(siString) arr = SI_ARR(siString, SI_STR("/home"), SI_STR("user"), SI_STR("Desktop"), SI_STR("RANDOM-ąčęėįšųū-òàèéç-йцукенвыамсч.txt"));
		str = si_stringJoin(arr, SI_STR("/"), alloc);
		si_printfLn("Joined str: \"%S\"", str);

		str = si_stringUpper(str, alloc);
		si_printfLn("Upper str: \"%S\"", str);

		str = si_stringLower(str, alloc);
		si_printfLn("Lower str: \"%S\"", str);
	}

	/* Based on: https://en.cppreference.com/w/cpp/container/map */
	{
		siMap(i32) m = si_mapMake(alloc, i32, {SI_STRC("CPU"), 10}, {SI_STRC("GPU"), 15}, {SI_STRC("RAM"), 20});
		print_map(SI_STR("1) Initial map: "), m);

		si_mapSetType(&m, SI_STR("CPU"), 25, i32);
		si_mapSet(&m, SI_STR("SSD"), &(i32){30});
		print_map(SI_STR("2) Updated map: "), m);

		si_printfLn("Key 'UPS' exists: %B", (si_mapGet(m, SI_STR("UPS")) != nil));

		si_mapErase(&m, SI_STR("GPU"));
		print_map(SI_STR("3) After erase: "), m);

		si_mapClear(&m);
		print_map(SI_STR("4) After clear: "), m);

		si_mapFree(m); /* Calls the needed free. */
	}
}
