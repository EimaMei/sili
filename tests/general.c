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
		TEST_EQ_INT(SI_KILO(1), 1024);
		TEST_EQ_INT(SI_MEGA(1), 1024 * 1024);
		TEST_EQ_INT(SI_GIGA(1), 1024 * 1024 * 1024);
		TEST_EQ_INT(SI_TERA(1), 1024 * 1024 * 1024 * 1024ll);

		TEST_EQ_INT(SI_BIT(63), 0x8000000000000000);
		TEST_EQ_INT(nil, (void*)0);

		isize m = transmute(isize, USIZE_MAX);
		TEST_EQ_INT(m, (isize)-1);

#if SI_ENDIAN_IS_LITTLE
		u32 value = 0x44434241;
#else
		u32 value = 0x41424344;
#endif
		TEST_EQ_INT(offsetof(randomStruct, three), 4 + si_sizeof(usize));
		TEST_EQ_INT(alignof(randomStruct), si_sizeof(usize));

		int buf1 = 8;
		int buf2 = 4;
		si_swap(buf1, buf2);
		TEST_EQ_INT(buf1, 4);
		TEST_EQ_INT(buf2, 8);

		i16 x = 0;
		for_range (i, INT16_MIN, 0) { x -= 1; }
		TEST_EQ_INT(x, INT16_MIN);

		u8 src[8] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
		u32 dst = 0;
		si_memcopy_s(SI_ARR_LEN((u8*)&dst, 4), &src, si_sizeof(src));
		TEST_EQ_HEX(dst, si_swap32be(0x00FF00FF));

		TEST_EQ_HEX(0x44434241, si_swap32le(value));
		TEST_EQ_HEX(0xFF00FF00FF00FF00, si_swap64(0x00FF00FF00FF00FF));
		TEST_EQ_HEX(0xFF, si_swap16(0xFF00));
	} SUCCEEDED();

	{
		isize ceil = si_alignForward(12, 8);
		TEST_EQ_INT(ceil, 16);

		for_range (i, 0, si_sizeof(usize) * 8 - 1) {
			TEST_EQ_TRUE(si_isPowerOfTwo((isize)SI_BIT(i)));
		}
		TEST_EQ_FALSE(si_isPowerOfTwo(0));
		TEST_EQ_FALSE(si_isPowerOfTwo(-238));
	} SUCCEEDED();

	{
		siAllocator alloc = si_allocatorHeap();
		TEST_EQ_PTR(alloc.proc, si_allocatorHeap_proc);
		TEST_EQ_NIL(alloc.data);

		void* ptr = si_alloc(alloc, SI_KILO(1));
		ptr = si_realloc(alloc, ptr, 0, SI_KILO(4));
		si_free(alloc, ptr);

		isize avail = si_allocatorMemAvailable(alloc);
		TEST_EQ_INT(avail, -1);
	} SUCCEEDED();

	{
		siArena aData = si_arenaMake(si_allocatorHeap(), SI_MEGA(1));
		TEST_EQ_PTR((void*)aData.alloc.proc, (void*)si_allocatorHeap_proc);
		TEST_EQ_INT(aData.offset, 0);
		TEST_EQ_INT(aData.capacity, SI_MEGA(1));
		TEST_NEQ_NIL(aData.ptr);

		siAllocator alloc = si_allocatorArena(&aData);
		TEST_EQ_PTR(alloc.proc, si_allocatorArena_proc);
		TEST_EQ_PTR(alloc.data, &aData);

		void* ptr = si_alloc(alloc, SI_KILO(1));
		TEST_EQ_INT(aData.offset, SI_KILO(1));
		TEST_EQ_PTR(ptr, aData.ptr);

		isize avail = si_allocatorMemAvailable(alloc);
		TEST_EQ_INT(avail, SI_MEGA(1) - SI_KILO(1));

		si_freeAll(alloc);
		TEST_EQ_INT(aData.offset, 0);

		si_arenaFree(&aData);
		TEST_EQ_NIL(aData.ptr);
		TEST_EQ_INT(aData.offset, 0);
		TEST_EQ_INT(aData.capacity, 0);
	} SUCCEEDED();

	{
		siPool pData = si_poolMake(si_allocatorHeap(), 3, 32);
		TEST_EQ_PTR(pData.alloc.proc, si_allocatorHeap_proc);
		TEST_EQ_INT(pData.chunkSize, 32);
		TEST_EQ_INT(pData.numChunks, 3);
		TEST_NEQ_NIL(pData.ptr);
		TEST_NEQ_NIL(pData.head);

		siAllocator alloc = si_allocatorPool(&pData);
		TEST_EQ_PTR(alloc.proc, si_allocatorPool_proc);
		TEST_EQ_PTR(alloc.data, &pData);

		void* previousHead = pData.head;
		u8* ptr = si_allocArray(alloc, u8, 24);
		TEST_EQ_PTR(ptr, si_pointerAdd(previousHead, si_sizeof(void*)));
		for_range (i, 0, 24) { TEST_EQ_CHAR(ptr[i], 0); }

		previousHead = pData.head;
		void* ptr2 = si_allocNonZeroed(alloc, 24);
		TEST_EQ_PTR(ptr2, si_pointerAdd(previousHead, si_sizeof(void*)));

		isize avail = si_allocatorMemAvailable(alloc);
		TEST_EQ_INT(avail, 32);

		si_allocNonZeroed(alloc, 24);

		siAllocationError err;
		avail = si_allocatorMemAvailable(alloc, &err);
		TEST_EQ_INT(avail, 0);
		TEST_EQ_INT(err, siAllocationError_None);

		si_freeAll(alloc);
		avail = si_allocatorMemAvailable(alloc);
		TEST_EQ_INT(avail, 32);

		u8 features = si_allocatorGetFeatures(alloc);
		TEST_EQ_INT(features, 0xF3); /* NOTE(EimaMei): '0b01110011' in hex. */

		si_poolFree(&pData);
		TEST_EQ_NIL(pData.ptr);
		TEST_EQ_INT(pData.numChunks, 0);
	} SUCCEEDED();

	{
		siOption(u64) opt = SI_OPT(u64, 19920216ULL);
		TEST_EQ_INT(opt.hasValue, 1);
		TEST_EQ_INT(opt.value, 19920216ULL);

		siError tmp = SI_STRUCT_ZERO;
		opt = SI_OPT_ERR(u64);
		TEST_EQ_INT(opt.hasValue, false);

		u64 res = si_optionalGetOrDefault(opt, UINT64_MAX);
		TEST_EQ_INT(res, UINT64_MAX);
	} SUCCEEDED();

	TEST_COMPLETE();
}


#if SI_COMPILER_MSVC
	#pragma warning(pop)
#endif

