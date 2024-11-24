#define SI_IMPLEMENTATION 1
#include <sili.h>


#define si_arrayPrintInt(array) \
	si_stringFromArray(array, (array.typeSize == 8) ? "%li" : "%i")

#define si_arrayPrintClr(array) \
	si_stringFromArray(array, "{%hhu, %hhu, %hhu}")


int main(void) {
	siAllocator alloc = si_allocatorMakeStack(256);

	si_print("Scope 1:\n");
	{
		siArray(i32) arrayStatic = SI_BUF(i32, 1, 2, 4, 8, 16, 32);
		si_printf("\tarray: \"%S\" or \"%S\"\n", si_arrayPrintInt(arrayStatic), si_stringFromArray(arrayStatic, "%i"));

		siBuffer array = si_arrayCopy(arrayStatic, alloc);
		si_printf("\t(array == arrayStatic) returns a '%B' boolean\n", si_arrayEqual(arrayStatic, array));

		b32 allocated = si_arrayAppend(&array, &(i32){64});
		si_printf("\tarray: \"%S\" (Was allocated: %B)\n", si_arrayPrintInt(array), allocated);

		i32 front, middle, back;
		si_arrayAtFront(array, &front);
		si_arrayAt(array, array.len / 2, &middle);
		si_arrayAtBack(array, &back);
		si_printf("\tfront: '%i', middle: '%i', back: '%i'\n", front, middle, back);
	}
	si_print("Scope 2:\n");
	{
		siArray(i32) array = SI_BUF(i32, INT32_MAX, INT8_MAX, UINT16_MAX, INT32_MAX, 128);
		si_printf(
			"\tarray: '%S', len: '%zd', capacity: '%zd'\n",
			si_stringFromArray(array, "%02#X"), array.len, array.capacity
		);

		i32 value = INT32_MAX;
		isize posFirst = si_arrayFind(array, &value),
			  posLast = si_arrayFindLast(array, &value);
		usize occurences = si_arrayFindCount(array, &value);

		si_printf(
			"\tThe integer '%02#X' was first found at index '%zd', last found at index '%zd', with '%zd' occurences in total.\n",
			value, posFirst, posLast, occurences
		);

		isize pos = si_arrayFind(array , &(i32){-123});
		si_printf("\tHowever, the integer '-123' wasn't found and so, the function returns '%zd'\n", pos);
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

		siArray(u32) array = si_arrayMake(alloc, siColor, {255, 0, 0}, {128, 128, 128}, {96, 255, 186}, {23, 204, 2});
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayReverse(array);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayClear(&array);
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));

		si_arrayFill(&array, 0, 4, &SI_RGB(255, 255, 255));
		si_printf("\tarray: \"%S\"\n", si_arrayPrintClr(array));
	}

	return 0;
}
