int test_on = false;

#define TEST_START() \
	test_on = true; \
	si_printfLn("%CTesting function '%S':%C", si_printColor3bitEx(siPrintColor3bit_Yellow, false, false), __func__);

#define SUCCEEDED() \
	si_printfLn("%S'%L' succeeded.", test_on ? "\t" : "", SI_CALLER_LOC)

#define TEST_COMPLETE() \
	test_on = false; \
	si_printfLn("%CTest '%S' has been completed!%C", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false), __func__);


#define ASSERT_FMT(condition, message, .../* fmt */) SI_STOPIF(!(condition), si_panic(SI_STR(#condition), SI_CALLER_LOC, message, __VA_ARGS__))
#define ASSERT(condition) ASSERT_FMT(condition, SI_STR(""), "")

#define TEST_EQ(arg1, arg2, format) \
	ASSERT_FMT((arg1) == (arg2), SI_STR(format " | " format), arg1, arg2)
#define TEST_NEQ(arg1, arg2, format) \
	ASSERT_FMT((arg1) != (arg2), SI_STR(format " | " format), arg1, arg2)


#define TEST_EQ_U64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%lu")
#define TEST_EQ_H64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%#lx")
#define TEST_EQ_I64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%li")
#define TEST_EQ_F64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%f")
#define TEST_EQ_CHAR(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%c")
#define TEST_EQ_PTR(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%p")
#define TEST_EQ_USIZE(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%zu")
#define TEST_EQ_ISIZE(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%zi")
#define TEST_EQ_U32(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%u")
#define TEST_EQ_STR(arg1, arg2) \
	ASSERT_FMT( \
		si_stringEqual(arg1, arg2), \
		SI_STR("len = {%zi, %zi}, str = {\"%s\", \"%s\"}"), arg1.len, arg2.len, arg1, arg2 \
	)
#define TEST_EQ_TRUE(arg1) \
	TEST_EQ_U32(arg1, true)
#define TEST_EQ_FALSE(arg1) \
	TEST_EQ_U32(arg1, false)

#define TEST_EQ_NIL(arg1) \
	TEST_EQ_PTR(arg1, nil)



#define TEST_NEQ_U64(arg1, arg2) \
	TEST_NEQ(arg1, arg2, "%lu")
#define TEST_NEQ_PTR(arg1, arg2) \
	TEST_NEQ(arg1, arg2, "%p")

#define TEST_NEQ_NIL(arg1) \
	TEST_NEQ_PTR(arg1, nil)
