#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>


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

int main(void) {
	{
		siString str = SI_STR(test_str);
		TEST_EQ_ISIZE(str.len, countof_str(test_str));
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_STR_LEN(test_str, 4);
		TEST_EQ_ISIZE(str.len, 4);
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_CSTR(((char*)test_str + 1));
		TEST_EQ_PTR(str.data, (const u8*)(test_str) + 1);
		TEST_EQ_ISIZE(str.len, countof_str(test_str) - 1);
		TEST_EQ_PTR(si_memcompare(str.data, (const u8*)test_str + 1, str.len), 0);

		str = SI_STR_EMPTY;
		TEST_N_EQ_PTR(str.data, nil);
		TEST_EQ_ISIZE(str.len, 0);

		str = SI_STR_NIL;
		TEST_EQ_PTR(str.data, (const void*)nil);
		TEST_EQ_ISIZE(str.len, 0);
	}
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
	}
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
	}
	/* NOTE(EimaMei): We malloc the buffer so that we could easily write a random
	 * value and from there figure out if said random value was overwritten. If
	 * it was, it means the functions are working as intended. */
	void* buf = si_mallocNonZeroed(SI_MEGA(1));
	siArena arena = si_arenaMakePtr(buf, SI_MEGA(1), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator alloc = si_allocatorArena(&arena);
	si_memset(buf, 0x93, SI_MEGA(1));
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
	}
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
	}
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
	}
	{
		siString str = SI_STR(test_str);
		isize i = si_stringFind(str, SI_STR("ty"));
		TEST_EQ_ISIZE(i, countof_str("qwer"));
		
		i = si_stringFindByte(str, '_');
		TEST_EQ_ISIZE(i, countof_str(test_str1));

		i = si_stringFindRune(str, 0x0433);
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“_йцукенн"));

		i = si_stringFindLast(str, SI_STR("ty"));
		TEST_EQ_ISIZE(i, countof_str("qwertyqwer"));
		
		i = si_stringFindLastByte(str, '_');
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“"));

		i = si_stringFindLastRune(str, 0x0433);
		TEST_EQ_ISIZE(i, countof_str("qwertyqwerty_ąčęėįšųū„“_йцукенн"));

		i = si_stringFindCount(str, SI_STR("_"));
		TEST_EQ_ISIZE(i, 2);
	}
	{
		siString str = SI_STR("DWgaOtP12df0");
		b32 res = si_stringEqual(str, SI_STR("dWgaf0"));
		TEST_EQ_ISIZE(res, 0);

		res = si_stringEqual(str, SI_STR("dWgaOtP12df0"));
		TEST_EQ_ISIZE(res, 0);
	
		res = si_stringEqual(str, SI_STR("DWgaOtP12df0"));
		TEST_EQ_ISIZE(res, 1);

		res = si_stringEqual(str, si_stringCopy(str, alloc));
		TEST_EQ_ISIZE(res, 1);

		i32 code = si_stringCompare(str, SI_STR("DWGAOTP12DF0"));
		SI_ASSERT(code > 0);
		
		code = si_stringCompare(SI_STR("DWGAOTP12DF0"), str);
		SI_ASSERT(code < 0);
	
		code = si_stringCompare(str, SI_STR("DWgaOtP12df0"));
		TEST_EQ_ISIZE(code, 0);

		code = si_stringCompare(str, si_stringCopy(str, alloc));
		TEST_EQ_ISIZE(code, 0);
	}
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
	}

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
	SI_UNUSED(global_str1); SI_UNUSED(global_str2); SI_UNUSED(global_str3);
	si_mfree(buf);
}
