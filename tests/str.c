#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>


#define test_str1 "qwerty"
#define test_str2 "ąčęėįšųū„“"
#define test_str3 "йцукеннгш"

#define test_str (test_str1 "_" test_str2 "_" test_str3)

/* NOTE(EimaMei): This should compile without issue. */
const siString global_str = SI_STRC("test");

int main(void) {
    {
        siString str = SI_STR(test_str);
        TEST_EQ_PTR(str.data, (const void*)(test_str));
        TEST_EQ_ISIZE(str.len, countof_str(test_str));
        TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

        str = SI_STR_LEN(test_str, 4);
        TEST_EQ_PTR(str.data, (const void*)(test_str));
        TEST_EQ_ISIZE(str.len, 4);
        TEST_EQ_PTR(si_memcompare(str.data, test_str, str.len), 0);

        str = SI_CSTR(test_str + 1);
        TEST_EQ_PTR(str.data, (const void*)(test_str + 1));
        TEST_EQ_ISIZE(str.len, countof_str(test_str) - 1);
        TEST_EQ_PTR(si_memcompare(str.data, test_str + 1, str.len), 0);

        str = SI_STR_EMPTY;
        TEST_EQ_PTR(str.data, (const void*)"");
        TEST_EQ_ISIZE(str.len, 0);

        str = SI_STR_NIL;
        TEST_EQ_PTR(str.data, (const void*)nil);
        TEST_EQ_ISIZE(str.len, 0);
    }
    #if SI_LANGUAGE_IS_CPP
    {
        siString str = SI_STR(test_str);
        for_range (i, 0, countof(test_str) - 1) {
            TEST_EQ_CHAR(str[i], (u8)test_str[i]);
        }

    }
    #endif


	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
}


#if SI_COMPILER_MSVC
	#pragma warning(pop)
#endif

