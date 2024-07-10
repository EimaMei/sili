#define SI_IMPLEMENTATION 1
#include <sili.h>

typedef struct randomStruct {
	usize one;
	char two;
	f32 three;
} randomStruct;


#define TEST_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) == (arg2), format " | " format, arg1, arg2)
#define TEST_N_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) != (arg2), format " | " format, arg1, arg2)


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


#define TEST_N_EQ_U64(arg1, arg2) \
	TEST_N_EQ(arg1, arg2, "%lu")



int main(void) {
	{
		TEST_EQ_U64(SI_KILO(1), 1024);
		TEST_EQ_U64(SI_MEGA(1), 1024 * 1024);
		TEST_EQ_U64(SI_GIGA(1), 1024 * 1024 * 1024);
		TEST_EQ_U64(SI_TERA(1), 1024 * 1024 * 1024 * 1024ull);

		TEST_EQ_U64(SI_BIT(63), 0x8000000000000000);
		TEST_EQ_U64(nil, (void*)0);

		isize m = si_transmuteEx(isize, USIZE_MAX, usize);
		TEST_EQ_I64(m, (isize)-1);

		u32 value;
		if (SI_LIKELY(SI_HOST_IS_LITTLE_ENDIAN)) {
			value = 0x44434241;
		}
		else {
			value = 0x41424344;
		}

		cstring str = "ABCD";
		TEST_EQ_U64(SI_TO_U32(str), value);

		TEST_EQ_U64(si_offsetof(randomStruct, three), 12);
		TEST_EQ_U64(si_alignof(randomStruct), 8);

		char* buf1 = si_buf(char, 'Q', 'W', 'E', 'R', 'T', 'Y', '\0');
		char* buf2 = "AZERTY";
		TEST_EQ_CHAR(buf1[0], 'Q');
		TEST_EQ_CHAR(buf1[6], '\0');

		si_swap(buf1, buf2);
		TEST_EQ_U64(strcmp(buf2, "QWERTY"), 0);
		TEST_EQ_U64(strcmp(buf1, "AZERTY"), strcmp(buf2, "QWERTY"));

		i16 x = 0;
		for_range (i, INT16_MIN, 0) {
			x -= 1;
		}
		TEST_EQ_I64(x, INT16_MIN);

		u64 src = 0x00FF00FF00FF00FF;
		u32 dst;
		memcpy_s(&dst, sizeof(dst), &src, sizeof(src));
		TEST_EQ_H64(dst, 0x00FF00FF);

		TEST_EQ_H64(0x44434241, si_swap32le(value));
		TEST_EQ_H64(0xFF00FF00FF00FF00, si_swap64(0x00FF00FF00FF00FF));
		TEST_EQ_H64(0xFF, si_swap16(0xFF00));

		u16 y[] = {0, UINT16_MAX};
		si_ptrMoveRight(&y[1], 2, 2);
		TEST_EQ_H64(y[0], 0xFFFF);

		y[0] = 0x8080;
		y[1] = 0;
		si_ptrMoveLeft(&y[0], 2, 2);
		TEST_EQ_H64(y[1], 0x8080);
	}
	si_print("Test 1 has been completed.\n");

	{
		usize ceil = si_alignCeilEx(12, 8);
		TEST_EQ_U64(ceil, 16);

		siAllocator alloc = si_allocatorMake(SI_MEGA(1));
		TEST_N_EQ_U64(alloc.ptr, 0);
		TEST_EQ_U64(alloc.offset, 0);
		TEST_EQ_U64(alloc.capacity, SI_MEGA(1));
		TEST_EQ_U64(alloc.isStack, false);

		b32 res = si_allocatorResize(&alloc, SI_KILO(1));
		TEST_EQ_U64(res, true);
		TEST_EQ_U64(alloc.capacity, SI_KILO(1));

		char x[128];
		siAllocator tmp = si_allocatorMakeTmp(x, countof(x));
		TEST_EQ_H64(tmp.ptr, (siByte*)x);
		TEST_EQ_U64(tmp.capacity, countof(x));

		si_malloc(&alloc, si_alignCeil(234));
		TEST_EQ_U64(si_allocatorAvailable(alloc), alloc.capacity - si_alignCeil(234));

		si_allocatorResetFrom(&alloc, 444);
		TEST_EQ_U64(alloc.offset, 444);
		si_allocatorFree(&alloc);
		TEST_EQ_H64(alloc.ptr, 0);

		siAllocator stack = si_allocatorMakeStack(32);
		TEST_EQ_U64(stack.isStack, true);
		si_allocatorPush(&stack, 'Q');
		si_allocatorPush(&stack, 'W');
		TEST_EQ_CHAR(stack.ptr[0], 'Q');
		TEST_EQ_CHAR(stack.ptr[1], 'W');
		TEST_EQ_CHAR(si_allocatorCurPtr(stack), &stack.ptr[2]);

		usize oldAmount = stack.offset;
		si_allocatorResetSub(&stack, 2);
		TEST_EQ_U64(stack.offset, oldAmount - 2);

		usize amounts[] = {SI_KILO(2), SI_KILO(4), SI_KILO(8), SI_MEGA(1)};
		b32 expectedResults[sizeof(amounts)] = {true, true, false, false};
		for_range (i, 0, countof(amounts)) {
			siAllocator alloc = si_allocatorMakeAny(SI_KILO(4),amounts[i]);
			TEST_EQ_U64(alloc.isStack, expectedResults[i]);

			si_allocatorFree(&alloc);
			TEST_EQ_H64(alloc.ptr, 0);
		}
	}
	si_print("Test 2 has been completed.\n");

	{
		usize* ptr1 = si_sallocItem(usize);
		*ptr1 = USIZE_MAX;
		usize* ptr2 = si_sallocCopy(*ptr1);
		TEST_EQ_H64(*ptr1, *ptr2);
		TEST_EQ_H64(*ptr1, USIZE_MAX);

		siAllocator allocator = si_allocatorMake(SI_KILO(1));
		randomStruct* alloc1 = si_mallocItem(&allocator, randomStruct);
		randomStruct* alloc2 = si_mallocArray(&allocator, randomStruct, 3);
		*alloc1 = (randomStruct){USIZE_MIN, INT8_MAX, FLOAT32_MIN};

		randomStruct* alloc3 = si_mallocCopy(&allocator, *alloc1);
		TEST_EQ_H64(alloc1->one, alloc3->one);
		TEST_EQ_H64(SI_TO_U64(&alloc1->two), SI_TO_U64(&alloc3->two));

		si_allocatorFree(&allocator);
		SI_UNUSED(alloc2);
	}
	si_print("Test 3 has been completed.\n");

	{
		siAny any = si_anyMakeType(i32, 23);
		TEST_EQ_I64(any.typeSize, sizeof((i32)23));

		siPoint p1 = SI_POINT(50, 50),
				p2 = (siPoint){28, 28};
		TEST_EQ_U64(si_pointCmp(p1, p2), 0);

		siColor c1 = SI_RGBA(128, 128, 128, 255),
				c2 = SI_RGB(255, 0, 0),
				c3 = SI_HEX(0x808080);
		TEST_EQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c3));
		TEST_N_EQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c2));

		siArea area = SI_AREA(2, 3);
		siRect r1 = SI_RECT(0, 1, 2, 3),
			   r2 = SI_RECT_A(4, 4, area),
			   r3 = SI_RECT_P(p1, 2, 3),
			   r4 = SI_RECT_PA(SI_POINT(0, 1), area);
		SI_UNUSED(r1), SI_UNUSED(r2), SI_UNUSED(r3), SI_UNUSED(r4);

		siVec2 v2 = SI_VEC2(2, 2);
		si_vec2Sub(&v2, SI_VEC2(-2, -2));
		TEST_EQ_F64(v2.x, 4);
		TEST_EQ_F64(v2.y, 4);
		si_vec2Add(&v2, SI_VEC2(-2, -2));
		TEST_EQ_F64(v2.x, 2);
		TEST_EQ_F64(v2.y, 2);
	}
	si_print("Test 4 has been completed.\n");

	{
		siOptional(u64) opt = si_optionalMake(19920216ULL);
		TEST_EQ_U64(opt->hasValue, 1);
		TEST_EQ_U64(opt->value, 19920216ULL);

		si_optionalReset(opt);
		TEST_EQ_U64(opt->value, 0);
		TEST_EQ_U64(opt->hasValue, false);

		opt = SI_OPTIONAL_NULL;

		u64 res = si_optionalGetOrDefault(opt, UINT64_MAX);
		TEST_EQ_U64(res, UINT64_MAX);
	}
	si_print("Test 5 has been completed.\n");


	si_printf("%CYTest '" __FILE__ "' has been completed!%C\n");
}
