int test_on = false;

#define TEST_START() \
	test_on = true; \
	si_printfLn("%CTesting function '%s':%C", si_printColor3bit(siPrintColor3bit_Yellow), SI_FUNC);

#define SUCCEEDED() \
	si_printfLn("%s'%L' succeeded.", test_on ? "\t" : "", SI_CALLER_LOC)

#define TEST_COMPLETE() \
	test_on = false; \
	si_printfLn("%CTest '%s' has been completed!%C", si_printColor3bit(siPrintColor3bit_Yellow, true), SI_FUNC);


#define ASSERT_FMT(condition, message, .../* fmt */) SI_STOPIF(!(condition), si_panic(SI_STR(#condition), SI_STR(message), SI_ARGS(__VA_ARGS__)))
#define ASSERT(condition) SI_STOPIF(!(condition), si_panic(SI_STR(#condition)))

#define TEST_EQ(arg1, arg2, format) \
	ASSERT_FMT((arg1) == (arg2), format " | " format, arg1, arg2)
#define TEST_NEQ(arg1, arg2, format) \
	ASSERT_FMT((arg1) != (arg2), format " | " format, arg1, arg2)


#define TEST_EQ_INT(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%i")
#define TEST_EQ_HEX(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%#X")
#define TEST_EQ_F64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%f")
#define TEST_EQ_CHAR(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%c")
#define TEST_EQ_PTR(arg1, arg2) \
	TEST_EQ((void*)arg1, (void*)arg2, "%p")
#define TEST_EQ_STR(arg1, arg2) \
	ASSERT_FMT( \
		si_stringEqual(arg1, arg2), \
		"len = {%i, %i}, str = {\"%s\", \"%s\"}", \
		arg1.len, arg2.len, arg1, arg2 \
	)
#define TEST_EQ_TRUE(arg) \
	TEST_EQ(true, arg, "%t")
#define TEST_EQ_FALSE(arg) \
	TEST_EQ(false, arg, "%t")

#define TEST_EQ_NIL(arg1) \
	TEST_EQ_PTR(arg1, nil)



#define TEST_NEQ_INT(arg1, arg2) \
	TEST_NEQ(arg1, arg2, "%i")
#define TEST_NEQ_PTR(arg1, arg2) \
	TEST_NEQ(arg1, arg2, "%p")

#define TEST_NEQ_NIL(arg1) \
	TEST_NEQ_PTR(arg1, nil)
