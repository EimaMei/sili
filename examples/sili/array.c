#define SI_IMPLEMENTATION 1
#include <sili.h>


#define si_dynamicArrayPrintInt(array) \
	si_stringFromArray(SI_ARR_DYN(array), (array.typeSize == 8) ? "%li" : "%i", SI_ARR_STACK(256))

#define si_dynamicArrayPrintClr(array) \
	si_stringFromArray(SI_ARR_DYN(array), "{%hhu, %hhu, %hhu}", SI_ARR_STACK(256))


int main(void) {
	siArena arena = si_arenaMakePtr(si_stackAlloc(256), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator alloc = si_allocatorArena(&arena);

	si_printLn("Scope 1:");
	{
		siArray(i32) buffer = SI_ARR(i32, 1, 2, 4, 8, 16, 32);
		si_printfLn("\tbuffer: \"%S\"", si_stringFromArray(buffer, "%i", SI_ARR_STACK(256)));

		si_print("\tyou can also print a buffer through a loop: {");
		i32 value;
		for_eachArr (value, buffer) {
			si_printf("%i ", value);
		}
		si_print("}\n");

		siDynamicArray(i32) array = si_dynamicArrayCopy(buffer, alloc);
		b32 res = si_arrayEqual(SI_ARR_DYN(array), buffer);
		si_printfLn("\t(array == buffer) returns a '%B' boolean", res);

		i64* lastElement = si_dynamicArrayAppend(&array, &(i32){64});
		si_printfLn("\tarray: \"%S\" (pointer to the lement: %p)", si_dynamicArrayPrintInt(array), lastElement);

		i32 front, middle, back;
		si_dynamicArrayAtFront(array, &front);
		si_dynamicArrayAtGet(array, array.len / 2, &middle);
		si_dynamicArrayAtBack(array, &back);
		si_printfLn("\tfront: '%i', middle: '%i', back: '%i'", front, middle, back);
	}

	si_printLn("Scope 2:");
	{
		siArray(i32) buffer = SI_ARR(i32, INT32_MAX, INT8_MAX, UINT16_MAX, INT32_MAX, 128);
		si_printfLn(
			"\tarray: '%S', len: '%zi'",
			si_stringFromArray(buffer, "%02#X", SI_ARR_STACK(256)),
			buffer.len
		);

		i32 value = INT32_MAX;
		isize posFirst = si_arrayFind(buffer, &value),
			  posLast = si_arrayFindLast(buffer, &value);
		isize occurences = si_arrayFindCount(buffer, &value);

		si_printfLn(
			"\tThe integer '%02#X' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.",
			value, posFirst, posLast, occurences
		);

		isize pos = si_arrayFind(buffer, &(i32){-123});
		si_printfLn("\tHowever, the integer '-123' wasn't found and so, the function returns '%zi'", pos);
	}

	si_printLn("Scope 3:");
	{
		siDynamicArray(i32) array = si_dynamicArrayMake(alloc, i32, 1, 1, 2, 0, 6, 6, 6);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayReplaceAll(array, &(i32){6}, &(i32){9});
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayEraseEx(&array, 1, 3);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayInsertEx(&array, 1, (i32[]){0xFF, 0xFFFF, 0x1991}, 3);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayEraseEx(&array, 1, 5);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintInt(array));
	}

	si_printLn("Scope 4:");
	{

		siDynamicArray(u32) array = si_dynamicArrayMake(alloc, siColor, {255, 0, 0, 255}, {128, 128, 128, 255}, {96, 255, 186, 255}, {23, 204, 2, 255});
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayReverse(array);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayClear(&array);
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayFill(&array, 0, 4, &SI_RGB(255, 255, 255));
		si_printfLn("\tarray: \"%S\"", si_dynamicArrayPrintClr(array));
	}
}
