#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>


void test_string(siAllocator alloc);
void test_builder(siAllocator alloc);
void test_conv(void);

int main(void) {
	siArena arena = si_arenaMake(si_allocatorHeap(), SI_MEGA(1));
	siAllocator alloc = si_allocatorArena(&arena);

	test_string(alloc);
	test_conv();
	test_builder(alloc);

	si_arenaFree(&arena);
}



#define test_str1 "qwertyqwerty"
#define test_str2 "ąčęėįšųū„“"
#define test_str3 "йцукеннгш"
#define test_str test_str1  "_" test_str2 "_" test_str3

i32 test_str_utf32[] = {
	'q', 'w', 'e', 'r', 't', 'y', 'q', 'w', 'e', 'r', 't', 'y',
	'_',
	0x0105, 0x010D, 0x0119, 0x0117, 0x012F, 0x0161, 0x0173, 0x016B, 0x201E, 0x201C,
	'_',
	0x0439, 0x0446, 0x0443, 0x043A, 0x0435, 0x043D, 0x043D, 0x0433, 0x0448
};

/* NOTE(EimaMei): These should compile without issue. */
const siString global_str1 = SI_STRC("test");
const siString global_str2 = SI_STRC_EMPTY;
const siString global_str3 = SI_STRC_NIL;


void test_string(siAllocator alloc) {
	SI_UNUSED(global_str1); SI_UNUSED(global_str2); SI_UNUSED(global_str3);
	TEST_START();

	{
		siString str = SI_STR(test_str);
		TEST_EQ_ISIZE(str.len, countof_str(test_str));
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_STR_LEN(test_str, 4);
		TEST_EQ_ISIZE(str.len, 4);
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_CSTR(((char*)test_str + 1));
		TEST_EQ_ISIZE(str.len, countof_str(test_str) - 1);
		TEST_EQ_PTR(si_memcompare(str.data, (const u8*)test_str + 1, str.len), 0);

		str = SI_STR_EMPTY;
		TEST_NEQ_PTR(str.data, nil);
		TEST_EQ_ISIZE(str.len, 0);

		str = SI_STR_NIL;
		TEST_EQ_PTR(str.data, (const void*)nil);
		TEST_EQ_ISIZE(str.len, 0);
	} SUCCEEDED();

	#if SI_LANGUAGE_IS_CPP
	{
		siString str = SI_STR(test_str);
		for_range (i, 0, str.len) {
			TEST_EQ_CHAR(str[i], (u8)test_str[i]);
		}

		for_range (i, 0, str.len) {
			siString slice;
			for_range (j, 0, str.len) {
				SI_STOPIF(i > j, break);

				slice = str(i, j);
				TEST_EQ_PTR(slice.data, str.data + i);
				TEST_EQ_ISIZE(slice.len, j - i);
			}

			slice = str.from(i);
			TEST_EQ_PTR(slice.data, str.data + i);
			TEST_EQ_ISIZE(slice.len, str.len - i);

			slice = str.to(i);
			TEST_EQ_PTR(slice.data, str.data);
			TEST_EQ_ISIZE(slice.len, i);

			for_range (j, 0, str.len) {
				SI_STOPIF(i + j > str.len, break);

				slice = str.substrLen(i, j);
				TEST_EQ_PTR(slice.data, str.data + i);
				TEST_EQ_ISIZE(slice.len, j);
			}
		}
	} SUCCEEDED();
	#endif

	{
		isize i = 0;
		siRune rune;
		for_eachStr (rune, SI_STR(test_str)) {
			TEST_EQ_CHAR(rune, test_str_utf32[i]);
			i += 1;
		}

		for_eachRevStr (rune, SI_STR(test_str)) {
			i -= 1;
			TEST_EQ_CHAR(rune, test_str_utf32[i]);
		}
	} SUCCEEDED();

	{
		siString str = SI_STR(test_str);

		siString res = si_stringCopy(str, alloc);
		TEST_EQ_STR(str, res);

		res = si_stringFromCStr(test_str, alloc);
		TEST_EQ_STR(str, res);

		char* cstr = si_stringToCStr(res, alloc);
		TEST_EQ_STR(SI_CSTR(cstr), res);
		TEST_EQ_CHAR(cstr[res.len], 0);

		/* si_stringToCStrEx */
	} SUCCEEDED();

	{
		siString str = SI_STR(test_str);

		siRune rune = si_stringAtFront(str);
		TEST_EQ_CHAR(rune, test_str_utf32[0]);

		rune = si_stringAtBack(str);
		TEST_EQ_CHAR(rune, test_str_utf32[countof(test_str_utf32) - 1]);

		const u8* ptr = si_stringBegin(str);
		TEST_EQ_PTR(ptr, str.data);

		ptr = si_stringEnd(str);
		TEST_EQ_PTR(ptr, str.data + str.len);


		str = SI_STR_EMPTY;

		rune = si_stringAtFront(str);
		TEST_EQ_CHAR(rune,  -1);

		rune = si_stringAtBack(str);
		TEST_EQ_CHAR(rune, -1);
	} SUCCEEDED();

	{
		siString str = SI_STR(test_str);

		for_range (i, 0, str.len) {
			siString slice;
			for_range (j, 0, str.len) {
				SI_STOPIF(i > j, break);

				slice = si_substr(str, i, j);
				TEST_EQ_PTR(slice.data, str.data + i);
				TEST_EQ_ISIZE(slice.len, j - i);
			}

			slice = si_substrFrom(str, i);
			TEST_EQ_PTR(slice.data, str.data + i);
			TEST_EQ_ISIZE(slice.len, str.len - i);

			slice = si_substrTo(str, i);
			TEST_EQ_PTR(slice.data, str.data);
			TEST_EQ_ISIZE(slice.len, i);

			for_range (j, 0, str.len) {
				SI_STOPIF(i + j > str.len, break);

				slice = si_substrLen(str, i, j);
				TEST_EQ_PTR(slice.data, str.data + i);
				TEST_EQ_ISIZE(slice.len, j);
			}
		}
	} SUCCEEDED();

	{
		siString str = SI_STR(test_str);

		isize i = si_stringFind(str, SI_STR("ty"));
		TEST_EQ_ISIZE(i, countof_str("qwer"));
		i = si_stringFind(str, SI_STR("sdfdf"));
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindByte(str, '_');
		TEST_EQ_ISIZE(i, countof_str(test_str1));
		i = si_stringFindByte(str, '0');
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindRune(str, 0x0433);
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“_йцукенн"));
		i = si_stringFindRune(str, '\0');
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindLast(str, SI_STR("ty"));
		TEST_EQ_ISIZE(i, countof_str("qwertyqwer"));
		i = si_stringFindLast(str, SI_STR("sdfdf"));
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindLastByte(str, '_');
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“"));
		i = si_stringFindLastByte(str, '0');
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindLastRune(str, 0x0433);
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“_йцукенн"));
		i = si_stringFindLastRune(str, '\0');
		TEST_EQ_ISIZE(i, -1);

		i = si_stringFindCount(str, SI_STR("_"));
		TEST_EQ_ISIZE(i, 2);
		i = si_stringFindCount(str, SI_STR("dfdjkf"));
		TEST_EQ_ISIZE(i, 0);
	} SUCCEEDED();

	{
		siString str = SI_STR("DWgaOtP12df0");
		bool res = si_stringEqual(str, SI_STR("dWgaf0"));
		TEST_EQ_ISIZE(res, 0);

		res = si_stringEqual(str, SI_STR("dWgaOtP12df0"));
		TEST_EQ_ISIZE(res, 0);

		res = si_stringEqual(str, SI_STR("DWgaOtP12df0"));
		TEST_EQ_ISIZE(res, 1);

		res = si_stringEqual(str, si_stringCopy(str, alloc));
		TEST_EQ_ISIZE(res, 1);

		i32 code = si_stringCompare(str, SI_STR("DWGAOTP12DF0"));
		ASSERT(code > 0);

		code = si_stringCompare(SI_STR("DWGAOTP12DF0"), str);
		ASSERT(code < 0);

		code = si_stringCompare(str, SI_STR("DWgaOtP12df0"));
		TEST_EQ_ISIZE(code, 0);

		code = si_stringCompare(str, si_stringCopy(str, alloc));
		TEST_EQ_ISIZE(code, 0);
	} SUCCEEDED();

	{
		#define trim_l "abcd"
		#define trim_s " \t\r\n\v\f"

		siString str = SI_STR(trim_l test_str trim_l);
		siString res = si_stringTrimLeft(str, SI_STR(trim_l));
		TEST_EQ_STR(res, SI_STR(test_str trim_l));

		res = si_stringTrimRight(str, SI_STR(trim_l));
		TEST_EQ_STR(res, SI_STR(trim_l test_str));

		res = si_stringTrim(str, SI_STR(trim_l));
		TEST_EQ_STR(res, SI_STR(test_str));

		str = SI_STR(trim_s test_str trim_s);
		res = si_stringStripLeft(str);
		TEST_EQ_STR(res, SI_STR(test_str trim_s));

		res = si_stringStripRight(str);
		TEST_EQ_STR(res, SI_STR(trim_s test_str));

		res = si_stringStrip(str);
		TEST_EQ_STR(res, SI_STR(test_str));

		res = si_stringUnquote(SI_STR("\"" test_str "\""));
		TEST_EQ_STR(res, SI_STR(test_str));
	} SUCCEEDED();

	{
		siString res;

		res = si_stringJoin(SI_ARR(siString, SI_STR(test_str1), SI_STR(test_str2), SI_STR(test_str3)), SI_STR("_"), alloc);
		TEST_EQ_STR(res, SI_STR(test_str));

		res = si_stringInsert(res, SI_STR("_vrains"), countof_str(test_str1), alloc);
		TEST_EQ_STR(res, SI_STR(test_str1 "_vrains_" test_str2 "_" test_str3));

		res = si_stringRemove(res, SI_STR("_"), 2, alloc);
		TEST_EQ_STR(res, SI_STR(test_str1 "vrains" test_str2 "_" test_str3));

		res = si_stringRemoveAll(SI_STR(test_str), SI_STR("_"), alloc);
		TEST_EQ_STR(res, SI_STR(test_str1 test_str2 test_str3));

		res = si_stringReplaceAll(SI_STR(test_str), SI_STR("_"), SI_STR("-"), alloc);
		TEST_EQ_STR(res, SI_STR(test_str1 "-" test_str2 "-" test_str3));

		res = si_stringReplace(res, SI_STR("-"), SI_STR("~"), 1, alloc);
		TEST_EQ_STR(res, SI_STR(test_str1 "~" test_str2 "-" test_str3));
	} SUCCEEDED();

	{
		siString str = SI_STR(test_str);
		const char* str_arr[] = {test_str1, test_str2, test_str3};
		siString nl = SI_STR("one\ntwo\nthree\nfour");
		const char* nl_arr[] = {"one", "two", "three", "four"};

		siArray(siString) arr = si_stringSplit(str, SI_STR("_"), alloc);
		siString* arrv = (siString*)arr.data;
		TEST_EQ_ISIZE(arr.len, countof(str_arr));
		for_range (i, 0, countof(str_arr)) {
			TEST_EQ_STR(arrv[i], SI_CSTR(str_arr[i]));
		}

		arr = si_stringSplitEx(str, SI_STR("_"), 1, alloc);
		arrv = (siString*)arr.data;
		TEST_EQ_ISIZE(arr.len, 2);
		TEST_EQ_STR(arrv[0], SI_STR(test_str1));
		TEST_EQ_STR(arrv[1], SI_STR(test_str2 "_" test_str3));

		arr = si_stringSplitLines(nl, alloc);
		arrv = (siString*)arr.data;
		TEST_EQ_ISIZE(arr.len, countof(nl_arr));
		for_range (i, 0, countof(nl_arr)) {
			TEST_EQ_STR(arrv[i], SI_CSTR(nl_arr[i]));
		}
		i32 i = 0;
		siString line;
		while (si_stringSplitIterate(&str, SI_STR("_"), &line)) {
			TEST_EQ_STR(line, SI_CSTR(str_arr[i]));
			i += 1;
		}
		TEST_EQ_ISIZE(i, countof(str_arr));

		i = 0;
		while (si_stringSplitLinesIterate(&nl, &line)) {
			TEST_EQ_STR(line, SI_CSTR(nl_arr[i]));
			i += 1;
		}
		TEST_EQ_ISIZE(i, countof(nl_arr));
	} SUCCEEDED();

	{
		siString res = si_stringReverse(SI_STR("helloWORLD123"), alloc);
		TEST_EQ_STR(res, SI_STR("321DLROWolleh"));

		res = si_stringReverse(SI_STR("ĄČĘĖĮŠŲŪ„“йцук"), alloc);
		TEST_EQ_STR(res, SI_STR("куцй“„ŪŲŠĮĖĘČĄ"));

		res = si_stringUpper(SI_STR("helloĄČĘ123йц"), alloc);
		TEST_EQ_STR(res, SI_STR("HELLOĄČĘ123ЙЦ"));

		res = si_stringLower(SI_STR("helloĄČĘ123йц"), alloc);
		TEST_EQ_STR(res, SI_STR("helloąčę123йц"));
	} SUCCEEDED();

	TEST_COMPLETE();
}


void test_builder(siAllocator alloc) {
	si_freeAll(alloc);
	TEST_EQ_ISIZE(si_allocatorGetAvailableMem(alloc), SI_MEGA(1));

	TEST_START();

	{
		siBuilder builder = si_builderMake(16, alloc);
		TEST_EQ_PTR(builder.alloc.proc, alloc.proc);
		TEST_EQ_PTR(builder.alloc.data, alloc.data);
		TEST_NEQ_PTR(builder.data, nil);
		TEST_EQ_ISIZE(builder.len, 0);
		TEST_EQ_ISIZE(builder.capacity, 16);
		TEST_EQ_ISIZE(builder.grow, 0);
		builder = si_builderMakeLen(5, 16, alloc);
		TEST_EQ_PTR(builder.alloc.proc, alloc.proc);
		TEST_EQ_PTR(builder.alloc.data, alloc.data);
		TEST_NEQ_PTR(builder.data, nil);
		TEST_EQ_ISIZE(builder.len, 5);
		TEST_EQ_ISIZE(builder.capacity, 16);
		TEST_EQ_ISIZE(builder.grow, 0);

		builder = si_builderMakeGrow(32, 16, alloc);
		TEST_EQ_PTR(builder.alloc.proc, alloc.proc);
		TEST_EQ_PTR(builder.alloc.data, alloc.data);
		TEST_NEQ_PTR(builder.data, nil);
		TEST_EQ_ISIZE(builder.len, 0);
		TEST_EQ_ISIZE(builder.capacity, 16);
		TEST_EQ_ISIZE(builder.grow, 32);

		builder = si_builderMakeNone(alloc);
		TEST_EQ_PTR(builder.alloc.proc, alloc.proc);
		TEST_EQ_PTR(builder.alloc.data, alloc.data);
		TEST_EQ_PTR(builder.data, nil);
		TEST_EQ_ISIZE(builder.len, 0);
		TEST_EQ_ISIZE(builder.capacity, 0);
		TEST_EQ_ISIZE(builder.grow, 0);

		builder = si_builderMakeEx(0, 0, si_allocatorGetAvailableMem(alloc) + 1, alloc);
		TEST_EQ_PTR(builder.alloc.proc, nil);
		TEST_EQ_PTR(builder.alloc.data, nil);
		TEST_EQ_PTR(builder.data, nil);
		TEST_EQ_ISIZE(builder.len, 0);
		TEST_EQ_ISIZE(builder.capacity, 0);
		TEST_EQ_ISIZE(builder.grow, 0);
	} SUCCEEDED();

	{
		siBuilder builder = si_builderMake(4, alloc);
		siAllocationError res = si_builderMakeSpaceFor(&builder, 2);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.capacity, 4);

		res = si_builderMakeSpaceFor(&builder, 4);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.capacity, 4);

		res = si_builderMakeSpaceFor(&builder, 8);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.capacity, 2 * (4 + 8));

		builder = si_builderMakeGrow(32, 4, alloc);
		res = si_builderMakeSpaceFor(&builder, 8);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.capacity, 4 + 32 + 8);

		res = si_builderMakeSpaceFor(&builder, si_allocatorGetAvailableMem(alloc) + 1);
		TEST_EQ_ISIZE(res, siAllocationError_OutOfMem);
		TEST_EQ_ISIZE(builder.capacity, 4 + 32 + 8);
	} SUCCEEDED();

	{
		siBuilder builder;
		siAllocationError res;

		builder = si_builderMake(4, alloc);

		res = si_builderWriteByte(&builder, 'A');
		TEST_EQ_ISIZE(builder.len, countof_str("A"));

		res = si_builderWriteBytes(&builder, "BCD", 3);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("A" "BCD"));

		res = si_builderWriteStr(&builder, SI_STR("EFG"));
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("A" "BCD" "EFG"));
		TEST_EQ_ISIZE(builder.capacity, 2 * (4 + 3));

		res = si_builderWriteRune(&builder, 0x0105);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("A" "BCD" "EFG" "ą"));

		siString str = si_builderToStr(builder);
		TEST_EQ_STR(SI_STR("ABCDEFGą"), str);

		char* cstr = si_builderToCstr(&builder);
		TEST_EQ_STR(SI_STR("ABCDEFGą"), SI_CSTR(cstr));
		TEST_EQ_U32(cstr[builder.len - 1], '\0');
	} SUCCEEDED();

	{
		siBuilder builder;
		siAllocationError res;

		builder = si_builderMakeGrow(32, 4, alloc);

		res = si_builderWriteStrQuoted(&builder, SI_STR("hello"));
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("\"hello\""));

		res = si_builderWriteStrQuotedEx(&builder, SI_STR("world"), '\'');
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("\"hello\"" "\'world\'"));

		res = si_builderWriteStrQuotedRune(&builder, SI_STR("labas, pasauli!"), 0x201E, 0x201C);
		TEST_EQ_ISIZE(res, 0);
		TEST_EQ_ISIZE(builder.len, countof_str("\"hello\"" "\'world\'" "„labas, pasauli!“"));

		siString str = si_builderToStr(builder);
		TEST_EQ_STR(SI_STR("\"hello\"" "\'world\'" "„labas, pasauli!“"), str);
	} SUCCEEDED();

	{
		siBuilder builder;
		siAllocationError res;

		builder = si_builderMakeEx(0, 64, 64, alloc);

		res = si_builderWriteInt(&builder, 123);
		TEST_EQ_ISIZE(res, 0);
		si_builderWriteByte(&builder, ' ');

		res = si_builderWriteInt(&builder, INT64_MIN);
		TEST_EQ_ISIZE(res, 0);
		si_builderWriteByte(&builder, ' ');

		res = si_builderWriteIntEx(&builder, 456, 2);
		TEST_EQ_ISIZE(res, 0);
		si_builderWriteByte(&builder, ' ');

		res = si_builderWriteIntEx(&builder, 456, 8);
		TEST_EQ_ISIZE(res, 0);
		si_builderWriteByte(&builder, ' ');

		res = si_builderWriteIntEx(&builder, 456, 12);
		TEST_EQ_ISIZE(res, 0);
		si_builderWriteByte(&builder, ' ');

		res = si_builderWriteIntEx(&builder, -456, 16);
		TEST_EQ_ISIZE(res, 0);

		siString str = si_builderToStr(builder);
		TEST_EQ_STR(SI_STR("123 -9223372036854775808 111001000 710 320 -1C8"), str);

		si_builderClear(&builder);
		TEST_EQ_ISIZE(si_builderToStr(builder).len, 0);

		res = si_builderWriteFloat(&builder, 3.14);
		TEST_EQ_ISIZE(res, 0);

		res = si_builderWriteFloatEx(&builder, 2.718, 10, 2);
		TEST_EQ_ISIZE(res, 0);

		res = si_builderWriteFloatEx(&builder, FLOAT32_MIN, 10, 46);
		TEST_EQ_ISIZE(res, 0);

		//res = si_builderWriteFloatEx(&builder, FLOAT64_MAX, 10, 0);
		//TEST_EQ_ISIZE(res, 0);

		str = si_builderToStr(builder);
		TEST_EQ_STR(str, SI_STR("3.140000" "2.72" "0.0000000000000000000000000000000000000117549435"));

		si_builderClear(&builder);
		si_builderWriteStr(&builder, SI_STR("aę!"));

		si_builderPopByte(&builder);
		si_builderPopRune(&builder);

		str = si_builderToStr(builder);
		TEST_EQ_STR(str, SI_STR("a"));
	} SUCCEEDED();

	TEST_COMPLETE();
}

SI_STATIC_ASSERT(SI_BASE_MAX == 32);


#if SI_COMPILER_MSVC
#pragma warning(disable : 4127)
#endif

void TEST_UINT(siString str, u64 expectedNum);
void TEST_UINT_EX(siString str, u64 expectedNum, isize expectedIndex);
void TEST_UINT_BASE(siString str, i32 base, u64 expectedNum, isize expectedIndex);

#define TEST_UINT(str, expectedNum) \
	TEST_UINT_EX(str, expectedNum, -2)

#define TEST_UINT_EX(str, expectedNum, expectedIndex) \
	TEST_UINT_BASE(str, -1, expectedNum, expectedIndex)

#define TEST_UINT_BASE(str,  base, expectedNum, expectedIndex) \
	do { \
		isize invalidIndex; \
		\
		u64 num = si_stringToUIntBase(str, base, &invalidIndex); \
		TEST_EQ_U64(num, expectedNum); \
		if (expectedIndex != -2) { \
			TEST_EQ_ISIZE(invalidIndex, expectedIndex); \
		} \
	} while (0)

#define TEST_INT(str, expectedNum) \
	TEST_INT_EX(str, expectedNum, -2)

#define TEST_INT_EX(str, expectedNum, expectedIndex) \
	TEST_INT_BASE(str, -1, expectedNum, expectedIndex)

#define TEST_INT_BASE(str,  base, expectedNum, expectedIndex) \
	do { \
		isize invalidIndex; \
		\
		i64 num = si_stringToIntBase(str, base, &invalidIndex); \
		TEST_EQ_I64(num, expectedNum); \
		if (expectedIndex != -2) { \
			TEST_EQ_ISIZE(invalidIndex, expectedIndex); \
		} \
	} while (0)

void test_conv(void) {
	TEST_START();

	{
		siString str = SI_STR("0123456789ABCDEFGHIJKLMNOPQRSTUV");
		TEST_EQ_STR(str, SI_CSTR((const char*)SI_NUM_TO_CHAR_TABLE_UPPER));
		TEST_EQ_ISIZE(str.len, SI_BASE_MAX);

		str = SI_STR("0123456789""abcdefghijklmnopqrstuv");
		TEST_EQ_STR(str, SI_CSTR((const char*)SI_NUM_TO_CHAR_TABLE_LOWER));
		TEST_EQ_ISIZE(str.len, SI_BASE_MAX);

		TEST_EQ_PTR(SI_NUM_TO_CHAR_TABLE, SI_NUM_TO_CHAR_TABLE_UPPER);

		si_numEnableUpper(false);
		TEST_EQ_PTR(SI_NUM_TO_CHAR_TABLE, SI_NUM_TO_CHAR_TABLE_LOWER);

		si_numEnableUpper(true);
		TEST_EQ_PTR(SI_NUM_TO_CHAR_TABLE, SI_NUM_TO_CHAR_TABLE_UPPER);

	} SUCCEEDED();

	{
		TEST_UINT(SI_STR("12345"), 12345);
		TEST_UINT(SI_STR("0"), 0);
		TEST_UINT(SI_STR("999_999_999_999_999_999"), 999999999999999999);
		TEST_UINT(SI_STR("18446744073709551615"), UINT64_MAX);
		TEST_UINT(SI_STR("18!446744073709551615"), 18);

		TEST_UINT_EX(SI_STR("1234a56"), 1234, 4);
		TEST_UINT_EX(SI_STR("9876543210"), 9876543210, -1);
		TEST_UINT_EX(SI_STR("123!456"), 123, 3);
		TEST_UINT_EX(SI_STR("abc123"), 0, 0);


		TEST_UINT_BASE(SI_STR("1V"), 32, 63, -1);
		TEST_UINT_BASE(SI_STR("VVVVVVVVVVVVV"), 32, UINT64_MAX, -1);

		TEST_UINT_BASE(SI_STR("0x1A3F"), -1, 0x1A3F, -1);
		TEST_UINT_BASE(SI_STR("0xFFAA"), -1, 0xFFAA, -1);
		TEST_UINT_BASE(SI_STR("0xFF!AA"), -1, 0xFF, 4);
		TEST_UINT_BASE(SI_STR("   FFFFFFFFFFFFFFFF   "), 16, UINT64_MAX, -1);

		TEST_UINT_BASE(SI_STR("0z123"), -1, 171, -1);
		TEST_UINT_BASE(SI_STR("123"), 12, 171, -1);
		TEST_UINT_BASE(SI_STR("839365134A2A240713"), 12, UINT64_MAX, -1);

		TEST_UINT_BASE(SI_STR("0d9876"), -1, 9876, -1);
		TEST_UINT_BASE(SI_STR("0d98!76"), -1, 98, 4);
		TEST_UINT_BASE(SI_STR("12345"), 10, 12345, -1);
		TEST_UINT_BASE(SI_STR("18446744073709551615"), 10, UINT64_MAX, -1);

		TEST_UINT_BASE(SI_STR("0o755"), -1, 0755, -1);
		TEST_UINT_BASE(SI_STR("0o75!5"), -1, 075, 4);
		TEST_UINT_BASE(SI_STR("755"), 8, 0755, -1);
		TEST_UINT_BASE(SI_STR("1777777777777777777777"), 8, UINT64_MAX, -1);

		TEST_UINT_BASE(SI_STR("+0b1101"), -1, 13, -1);
		TEST_UINT_BASE(SI_STR("0b11!01"), -1, 3, 4);
		TEST_UINT_BASE(SI_STR("1101"), 2, 13, -1);
		TEST_UINT_BASE(SI_STR("11012"), 2, 13, 4);
		TEST_UINT_BASE(SI_STR("1111111111111111111111111111111111111111111111111111111111111111"), 2, UINT64_MAX, -1);
	} SUCCEEDED();

	{
		TEST_INT(SI_STR("12345"), 12345);
		TEST_INT(SI_STR("0"), 0);
		TEST_INT(SI_STR("-999_999_999_999"), -999999999999);
		TEST_INT(SI_STR("-9223372036854775808"), INT64_MIN);
		TEST_INT(SI_STR("9223372036854775807"), INT64_MAX);
		TEST_INT(SI_STR("-42!999"), -42);

		TEST_INT_EX(SI_STR("1234a56"), 1234, 4);
		TEST_INT_EX(SI_STR("-98765z43210"), -98765, 6);
		TEST_INT_EX(SI_STR("123!456"), 123, 3);
		TEST_INT_EX(SI_STR("-abc123"), 0, 1);

		TEST_INT_BASE(SI_STR("1V"), 32, 63, -1);
		TEST_INT_BASE(SI_STR("7VVVVVVVVVVVV"), 32, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("-8000000000000"), 32, INT64_MIN, -1);

		TEST_INT_BASE(SI_STR("0x1A3F"), -1, 0x1A3F, -1);
		TEST_INT_BASE(SI_STR("-0xFFAA"), -1, -0xFFAA, -1);
		TEST_INT_BASE(SI_STR("   7FFFFFFFFFFFFFFF   "), 16, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("   -8000000000000000   "), 16, INT64_MIN, -1);

		TEST_INT_BASE(SI_STR("0z123"), -1, 171, -1);
		TEST_INT_BASE(SI_STR("-0z123"), -1, -171, -1);
		TEST_INT_BASE(SI_STR("123"), 12, 171, -1);
		TEST_INT_BASE(SI_STR("-123"), 12, -171, -1);
		TEST_INT_BASE(SI_STR("41A792678515120367"), 12, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("-41A792678515120368"), 12, INT64_MIN, -1);

		TEST_INT_BASE(SI_STR("0d-9876"), -1, 0, 2);
		TEST_INT_BASE(SI_STR("-0d9876"), -1, -9876, -1);
		TEST_INT_BASE(SI_STR("-12345"), 10, -12345, -1);
		TEST_INT_BASE(SI_STR("9223372036854775807"), 10, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("-9223372036854775808"), 10, INT64_MIN, -1);

		TEST_INT_BASE(SI_STR("0o755"), -1, 0755, -1);
		TEST_INT_BASE(SI_STR("-0o755"), -1, -0755, -1);
		TEST_INT_BASE(SI_STR("777777777777777777777"), 8, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("-1000000000000000000000"), 8, INT64_MIN, -1);

		TEST_INT_BASE(SI_STR("0b1101"), -1, 13, -1);
		TEST_INT_BASE(SI_STR("-0b1101"), -1, -13, -1);
		TEST_INT_BASE(SI_STR("1101"), 2, 13, -1);
		TEST_INT_BASE(SI_STR("-1101"), 2, -13, -1);
		TEST_INT_BASE(SI_STR("111111111111111111111111111111111111111111111111111111111111111"), 2, INT64_MAX, -1);
		TEST_INT_BASE(SI_STR("-1000000000000000000000000000000000000000000000000000000000000000"), 2, INT64_MIN, -1);
	} SUCCEEDED();

	{
		siString str;

		str = si_stringFromBool(true);
		TEST_EQ_STR(str, SI_STR("true"));
		str = si_stringFromBool(false);
		TEST_EQ_STR(str, SI_STR("false"));

		siString trues[] = {SI_STR("1"), SI_STR("t"), SI_STR("T"), SI_STR("true"), SI_STR("True")};
		siString falses[countof(trues)] = {SI_STR("0"), SI_STR("f"), SI_STR("F"), SI_STR("false"), SI_STR("False")};

		for_range (i, 0, countof(trues)) {
			b32 res = si_stringToBool(trues[i]);
			TEST_EQ_U32(res, true);

			res = si_stringToBool(falses[i]);
			TEST_EQ_U32(res, false);
		}

		b32 res = si_stringToBool(SI_STR("tru"));
		TEST_EQ_U32(res, UINT32_MAX);
	} SUCCEEDED();

	TEST_COMPLETE();
}
