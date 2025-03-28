#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>


#define test_str1 "qwerty"
#define test_str2 "ąčęėįšųū„“"
#define test_str3 "йцукеннгш"
#define test_str (test_str1 "_" test_str2 "_" test_str3)
const u8* test_str_static = (const u8*)test_str;

i32 test_str_utf32[] = {
	'q', 'w', 'e', 'r', 't', 'y',
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
		TEST_EQ_PTR(str.data, test_str_static);
		TEST_EQ_ISIZE(str.len, countof_str(test_str));
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_STR_LEN(test_str, 4);
		TEST_EQ_PTR(str.data, test_str_static);
		TEST_EQ_ISIZE(str.len, 4);
		TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

		str = SI_CSTR(test_str + 1);
		TEST_EQ_PTR(str.data, (const void*)(test_str + 1));
		TEST_EQ_ISIZE(str.len, countof_str(test_str) - 1);
		TEST_EQ_PTR(si_memcompare(str.data, test_str + 1, str.len), 0);

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



	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
	SI_UNUSED(global_str1); SI_UNUSED(global_str2); SI_UNUSED(global_str3);
	si_mfree(buf);
}
