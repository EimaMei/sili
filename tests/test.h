int test_on = false;

#define TEST_START() \
	test_on = true; \
	si_printfLn("%CTesting function '%s':%C", si_printColor3bitEx(siPrintColor3bit_Yellow, false, false), __func__);

#define SUCCEEDED() \
	si_printfLn("%s'%L' succeeded.", test_on ? "\t" : "", SI_CALLER_LOC)

#define TEST_COMPLETE() \
	test_on = false; \
	si_printfLn("%CTest '%s' has been completed!%C", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false), __func__);



#define TEST_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) == (arg2), SI_STR(format " | " format), arg1, arg2)
#define TEST_N_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) != (arg2), SI_STR(format " | " format), arg1, arg2)


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
	SI_ASSERT_FMT(arg1.len == arg2.len && \
		si_memcompare(arg1.data, arg2.data, arg1.len) == 0, \
		SI_STR("len = {%zi, %zi}, str = {\"%S\", \"%S\"}"), arg1.len, arg2.len, arg1, arg2 \
	)

#define TEST_N_EQ_U64(arg1, arg2) \
	TEST_N_EQ(arg1, arg2, "%lu")
#define TEST_N_EQ_PTR(arg1, arg2) \
	TEST_N_EQ(arg1, arg2, "%p")
