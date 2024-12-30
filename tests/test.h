#define TEST_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) == (arg2), format" | "format, arg1, arg2)
#define TEST_N_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) != (arg2), format" | "format, arg1, arg2)


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
#define TEST_EQ_U32(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%u")


#define TEST_N_EQ_U64(arg1, arg2) \
	TEST_N_EQ(arg1, arg2, "%lu")
