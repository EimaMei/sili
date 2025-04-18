#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>

#if SI_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable : 4127)
#endif

typedef struct randomStruct {
	usize one;
	char two;
	f32 three;
} randomStruct;



int main(void) {
	TEST_START();

	{
		TEST_EQ_U64(SI_KILO(1), 1024);
		TEST_EQ_U64(SI_MEGA(1), 1024 * 1024);
		TEST_EQ_U64(SI_GIGA(1), 1024 * 1024 * 1024);
		TEST_EQ_U64(SI_TERA(1), 1024 * 1024 * 1024 * 1024ll);

		TEST_EQ_U64(SI_BIT(63), 0x8000000000000000);
		TEST_EQ_U64(nil, (void*)0);

		isize m = si_transmute(isize, USIZE_MAX, usize);
		TEST_EQ_I64(m, (isize)-1);

#if SI_ENDIAN_IS_LITTLE
		u32 value = 0x44434241;
#else
		u32 value = 0x41424344;
#endif
		char str[4] = {'A', 'B', 'C', 'D'};
		TEST_EQ_U64(SI_TO_U32(str), value);

		TEST_EQ_U64(offsetof(randomStruct, three), 4 + si_sizeof(usize));
		TEST_EQ_U64(alignof(randomStruct), si_sizeof(usize));

		int buf1 = 8;
		int buf2 = 4;
		si_swap(buf1, buf2);
		TEST_EQ_U32(buf1, 4);
		TEST_EQ_U32(buf2, 8);

		i16 x = 0;
		for_range (i, INT16_MIN, 0) { x -= 1; }
		TEST_EQ_I64(x, INT16_MIN);

		u8 src[8] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
		u32 dst = 0;
		si_memcopy_s(SI_ARR_LEN((u8*)&dst, 4), &src, si_sizeof(src));
		TEST_EQ_H64(dst, si_swap32be(0x00FF00FF));

		TEST_EQ_H64(0x44434241, si_swap32le(value));
		TEST_EQ_H64(0xFF00FF00FF00FF00, si_swap64(0x00FF00FF00FF00FF));
		TEST_EQ_H64(0xFF, si_swap16(0xFF00));

		u16 y[] = {0, UINT16_MAX};
		si_memmoveLeft(&y[1], 2, 2);
		TEST_EQ_H64(y[0], 0xFFFF);

		y[0] = 0x8080;
		y[1] = 0;
		si_memmoveRight(&y[0], 2, 2);
		TEST_EQ_H64(y[1], 0x8080);
	}
	si_print("Test 1 has been completed.\n");

	{
		isize ceil = si_alignForward(12, 8);
		TEST_EQ_USIZE(ceil, 16);

		for_range (i, 0, si_sizeof(usize) * 8 - 1) {
			TEST_EQ_TRUE(si_isPowerOfTwo((isize)SI_BIT(i)));
		}
		TEST_EQ_FALSE(si_isPowerOfTwo(0));
		TEST_EQ_FALSE(si_isPowerOfTwo(-238));
	}
	si_print("Test 2 has been completed.\n");

	{
		siAllocator alloc = si_allocatorHeap();
		TEST_EQ_PTR(alloc.proc, si_allocatorHeap_proc);
		TEST_EQ_NIL(alloc.data);

		void* ptr = si_alloc(alloc, SI_KILO(1));
		ptr = si_realloc(alloc, ptr, 0, SI_KILO(4));
		si_free(alloc, ptr);

		isize avail = si_allocatorGetAvailableMem(alloc);
		TEST_EQ_ISIZE(avail, -1);
	}
	si_print("Test 3 has been completed.\n");

	{
		siArena aData = si_arenaMake(si_allocatorHeap(), SI_MEGA(1));
		TEST_EQ_PTR(aData.alloc.proc, si_allocatorHeap_proc);
		TEST_EQ_USIZE(aData.offset, 0);
		TEST_EQ_USIZE(aData.capacity, SI_MEGA(1));
		TEST_EQ_U32(aData.alignment, SI_DEFAULT_MEMORY_ALIGNMENT);
		TEST_NEQ_NIL(aData.ptr);

		siAllocator alloc = si_allocatorArena(&aData);
		TEST_EQ_PTR(alloc.proc, si_allocatorArena_proc);
		TEST_EQ_PTR(alloc.data, &aData);

		void* ptr = si_alloc(alloc, SI_KILO(1));
		TEST_EQ_USIZE(aData.offset, SI_KILO(1));
		TEST_EQ_PTR(ptr, aData.ptr);

		isize avail = si_allocatorGetAvailableMem(alloc);
		TEST_EQ_USIZE(avail, SI_MEGA(1) - SI_KILO(1));

		si_freeAll(alloc);
		TEST_EQ_USIZE(aData.offset, 0);

		si_arenaFree(&aData);
		TEST_EQ_NIL(aData.ptr);
		TEST_EQ_USIZE(aData.offset, 0);
		TEST_EQ_USIZE(aData.capacity, 0);
	}
	si_print("Test 4 has been completed.\n");

	{
		siPool pData = si_poolMake(si_allocatorHeap(), 3, 32);
		TEST_EQ_PTR(pData.alloc.proc, si_allocatorHeap_proc);
		TEST_EQ_U32(pData.alignment, SI_DEFAULT_MEMORY_ALIGNMENT);
		TEST_EQ_USIZE(pData.chunkSize, 32);
		TEST_EQ_USIZE(pData.numChunks, 3);
		TEST_NEQ_NIL(pData.ptr);
		TEST_NEQ_NIL(pData.head);

		siAllocator alloc = si_allocatorPool(&pData);
		TEST_EQ_PTR(alloc.proc, si_allocatorPool_proc);
		TEST_EQ_PTR(alloc.data, &pData);

		void* previousHead = pData.head;
		u8* ptr = si_allocArray(alloc, u8, 24);
		TEST_EQ_PTR(ptr, si_pointerAdd(previousHead, si_sizeof(void*)));
		for_range (i, 0, 24) {
			TEST_EQ_CHAR(ptr[i], 0);
		}

		previousHead = pData.head;
		void* ptr2 = si_allocNonZeroed(alloc, 24);
		TEST_EQ_PTR(ptr2, si_pointerAdd(previousHead, si_sizeof(void*)));

		isize avail = si_allocatorGetAvailableMem(alloc);
		TEST_EQ_USIZE(avail, 32);

		si_allocNonZeroed(alloc, 24);

		avail = si_allocatorGetAvailableMem(alloc);
		TEST_EQ_USIZE(avail, 0);

		si_freeAll(alloc);
		avail = si_allocatorGetAvailableMem(alloc);
		TEST_EQ_USIZE(avail, 32);

		u8 features = si_allocatorGetFeatures(alloc);
		TEST_EQ_U32(features, 0xF3); /* NOTE(EimaMei): '0b01110011' in hex. */

		si_poolFree(&pData);
		TEST_EQ_NIL(pData.ptr);
		TEST_EQ_USIZE(pData.numChunks, 0);
	}
	si_print("Test 5 has been completed.\n");

	{
		siPoint p1 = SI_POINT(50, 50),
				p2 = SI_POINT(28, 28);
		TEST_EQ_U64(si_pointCmp(p1, p2), 0);

		siColor c1 = SI_RGBA(128, 128, 128, 255),
				c2 = SI_RGB(255, 0, 0),
				c3 = SI_HEX(0x808080);
		TEST_EQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c3));
		TEST_NEQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c2));

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
	si_print("Test 6 has been completed.\n");

	{
		siOption(u64) opt = SI_OPT(u64, 19920216ULL);
		TEST_EQ_U64(opt.hasValue, 1);
		TEST_EQ_U64(opt.data.value, 19920216ULL);

		siError tmp = SI_STRUCT_ZERO;
		tmp.code = 40;
		opt = SI_OPT_ERR(u64, tmp);
		TEST_EQ_I64(opt.data.error.code, 40);

		u64 res = si_optionalGetOrDefault(opt, UINT64_MAX);
		TEST_EQ_U64(res, UINT64_MAX);

		#if SI_STANDARD_CHECK_MIN(C, C11)
			opt = SI_OPT(u64, 19920216ULL);
			TEST_EQ_U64(opt.hasValue, 1);
			TEST_EQ_U64(opt.value, 19920216ULL);

			tmp.code = 40;
			opt = SI_OPT_ERR(u64, tmp);
			TEST_EQ_I64(opt.error.code, 40);
		#endif

	}
	si_print("Test 7 has been completed.\n");


	TEST_COMPLETE();
}


#if SI_COMPILER_MSVC
	#pragma warning(pop)
#endif

