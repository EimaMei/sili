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
		si_printfLn("\tstr: \"%s\" or \"%.*s\", same thing.", strStatic, strStatic.len, strStatic.data);

		siString str = si_stringCopy(strStatic, alloc);
		si_printfLn("\t(str == strStatic) returns a '%t' boolean", si_stringEqual(str, strStatic));

	}

	si_printLn("Scope 2:");
	{
		siBuilder b = si_builderMake(alloc, 256);
		si_printfLn("\tcapacity: '%i' len: '%i' grow: '%i'", b.capacity, b.len, b.grow);

		si_builderWriteStr(&b, SI_STR("Dynamically allocated string"));
		si_printfLn("\tstr: '%.*s', len: '%i'", b.len, b.data, b.len);

		si_builderWriteByte(&b, '.');
		si_printfLn("\tstr: '%s', len: '%i'", si_builderToStr(b), b.len);

		si_builderWriteRune(&b, U'й');
		si_printfLn("\tstr: '%s', len: '%i'", si_builderToStr(b), b.len);

		si_builderWriteStrQuoted(&b, SI_STR("Hello world."));
		si_printfLn("\tstr: '%s', len: '%i'", si_builderToStr(b), b.len);

		si_builderWriteStrQuotedRune(&b, SI_STR("Labas, pasauli!"),  U'„', U'“');
		si_printfLn("\tstr: '%s', len: '%i'", si_builderToStr(b), b.len);

		siString str = si_builderToStr(b);
		siRune front = si_stringAtFront(str);
		siRune back = si_stringAtBack(str);
		si_printfLn("\tfront: '%r', back: '%c'", front, back);
	}

	si_printfLn("Scope 3:");
	{
		siString str = SI_STR("Geri vyrai geroj girioj gerą girą gėrė ir gerdami gyrė: geriems vyrams geroj girioj gerą girą gera gert.");
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

		siString str_ger = SI_STR("ger");
		isize posFirst = si_stringFind(str, str_ger),
			  posLast = si_stringFindLast(str, str_ger);
		isize occurences = si_stringFindCount(str, str_ger);

		si_printfLn(
			"\tThe substring '%s' was first found at index '%i', last found at index '%i', with '%i' occurences in total.",
			str_ger, posFirst, posLast, occurences
		);

		isize pos = si_stringFind(str, SI_STR("Žąsys"));
		si_printfLn("\tHowever, the substring 'Žąsys' wasn't found and so, the function returns '%i'", pos);
	}

	si_printLn("Scope 4:");
	{
		siString str = SI_STR("smaug giganteus");
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

		str = si_stringTrim(str, SI_STR("s"));
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

		str = si_stringInsert(str, SI_STR("the "), si_countof_str("maug "), alloc);
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

		str = si_stringRemoveAll(str, SI_STR("gigant"), alloc);
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

	}

	si_printLn("Scope 5:");
	{
		siString str = SI_STR("one.two.three.four.five");
		si_printfLn("\tstr: '%s', len: '%i'", str, str.len);

		siArray(siString) list = si_stringSplit(str, SI_STR("."), alloc);
		si_printfLn("\tElements: %s", si_stringFromArray(SI_ARR_STACK(64), list, "%s", si_typeid(siString)));
	}

	si_printLn("Scope 6:");
	{
		siString str = SI_STR("\t       dnuora gniliart        ");
		si_printfLn("\tBefore: '%s' (len: '%i')", str, str.len);

		str = si_stringStrip(str);
		si_printfLn("\tAfter: '%s' (len: '%i')", str, str.len);

		siString reverse = si_stringReverse(str, alloc);
		si_printfLn("\t'str' in reverse: '%s'", reverse);
	}

	si_freeAll(alloc);
}


void print_map(siString comment, siMap(i32) map) {
	si_print(comment);

	siString key;
	i32 value;
    for_eachMapEx (key, value, map) {
		si_printf("[%s] = %i; ", key, value);
	}
	si_print("\n");
}

void example2(siAllocator alloc) {
	si_printfLn("==============\n\n==============\nExample 2:\n");

	siString str;
	{
		siArray(u8) buf = si_arrayMakeReserve(u8, 4, alloc);
		str = si_stringFromInt(buf, -342);
		si_printfLn("str: \"%s\"", str);

		i64 num = si_stringToInt(SI_STR("  9300  "));
		si_printfLn("num: %i", num);

		str = si_stringFromFloat(si_arrayMakeReserve(u8, 128, alloc), (f64)FLOAT32_MAX);
		si_printfLn("str: %s", str);

		siArray(siString) arr = SI_ARR(siString, SI_STR("/home"), SI_STR("user"), SI_STR("Desktop"), SI_STR("RANDOM-ąčęėįšųū-òàèéç-йцукенвыамсч.txt"));
		str = si_stringJoin(arr, SI_STR("/"), alloc);
		si_printfLn("Joined str: \"%s\"", str);

		str = si_stringUpper(str, alloc);
		si_printfLn("Upper str: \"%s\"", str);

		str = si_stringLower(str, alloc);
		si_printfLn("Lower str: \"%s\"", str);
	}

	/* Based on: https://en.cppreference.com/w/cpp/container/map */
	{
		siMap(i32) m = si_mapMake(alloc, i32, {SI_STRC("CPU"), 10}, {SI_STRC("GPU"), 15}, {SI_STRC("RAM"), 20});
		print_map(SI_STR("1) Initial map: "), m);

		si_mapSetItem(&m, SI_STR("CPU"), 25, i32);
		si_mapSetItem(&m, SI_STR("SSD"), 30, i32);
		print_map(SI_STR("2) Updated map: "), m);

		si_printfLn("Key 'UPS' exists: %t", (si_mapGet(m, SI_STR("UPS")) != nil));

		si_mapErase(&m, SI_STR("GPU"));
		print_map(SI_STR("3) After erase: "), m);

		si_mapClear(&m);
		print_map(SI_STR("4) After clear: "), m);

		si_mapFree(m); /* We don't need to call free here (in fact this results in
						a printed error), since the allocator is an arena and free
						cannot be implemented. */
	}
}
