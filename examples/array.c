#define SI_IMPLEMENTATION 1
#include <sili.h>


#define si_arrayPrintInt(array) \
	si_stringFromBuffer(SI_BUF_ARR(array), (array.typeSize == 8) ? "%li" : "%i", SI_BUF_STACK(256))

#define si_arrayPrintClr(array) \
	si_stringFromBuffer(SI_BUF_ARR(array), "{%hhu, %hhu, %hhu}", SI_BUF_STACK(256))


int main(void) {
	siAllocatorArena arena = si_arenaMakePtr(si_stackAlloc(256), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator alloc = si_allocatorArena(&arena);

	si_print("Scope 1:\n");
	{
		siBuffer(i32) buffer = SI_BUF(i32, 1, 2, 4, 8, 16, 32);
		si_printf("\buffer: \"%S\"\n", si_stringFromBuffer(buffer, "%i", SI_BUF_STACK(256)));

		siArray(i32) array = si_arrayCopy(buffer, alloc);
		b32 res = si_bufferEqual(SI_BUF_ARR(array), buffer);
		si_printf("\t(array == buffer) returns a '%B' boolean\n", res);

		b32 allocated = si_arrayAppend(&array, &(i32){64});
		si_printf("\tarray: \"%S\" (Was allocated: %B)\n", si_arrayPrintInt(array), allocated);

		i32 front, middle, back;
		si_arrayAtFront(array, &front);
		si_arrayAtGet(array, array.len / 2, &middle);
		si_arrayAtBack(array, &back);
		si_printf("\tfront: '%i', middle: '%i', back: '%i'\n", front, middle, back);
	}

	si_print("Scope 2:\n");
	{
		siBuffer(i32) buffer = SI_BUF(i32, INT32_MAX, INT8_MAX, UINT16_MAX, INT32_MAX, 128);
		si_printf(
			"\tarray: '%S', len: '%zi'\n",
			si_stringFromBuffer(buffer, "%02#X", SI_BUF_STACK(256)),
			buffer.len
		);

		i32 value = INT32_MAX;
		isize posFirst = si_bufferFind(buffer, &value),
			  posLast = si_bufferFindLast(buffer, &value);
		isize occurences = si_bufferFindCount(buffer, &value);

		si_printf(
			"\tThe integer '%02#X' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.\n",
			value, posFirst, posLast, occurences
		);

		isize pos = si_bufferFind(buffer, &(i32){-123});
		si_printf("\tHowever, the integer '-123' wasn't found and so, the function returns '%zi'\n", pos);
	}

	si_print("Scope 3:\n");
	{
		siArray(i32) array = si_arrayMake(alloc, i32, 1, 1, 2, 0, 6, 6, 6);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintInt(array));

		si_arrayReplaceAll(array, &(i32){6}, &(i32){9});
		si_printf("\tarray: \"%S\"\n", si_arrayPrintInt(array));

		si_arrayEraseEx(&array, 1, 3);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintInt(array));

		si_arrayInsertEx(&array, 1, (i32[]){0xFF, 0xFFFF, 0x1991}, 3);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintInt(array));

		si_arrayEraseEx(&array, 1, 5);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintInt(array));
	}

	si_print("Scope 3:\n");
	{

		siArray(u32) array = si_arrayMake(alloc, siColor, {255, 0, 0, 255}, {128, 128, 128, 255}, {96, 255, 186, 255}, {23, 204, 2, 255});
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayReverse(array);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayClear(&array);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayFill(&array, 0, 4, &SI_RGB(255, 255, 255));
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));
	}
}
