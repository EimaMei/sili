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
		si_printfLn("\tbuffer: \"%s\"", si_stringFromArray(buffer, "%i", SI_ARR_STACK(256)));

		si_print("\tyou can also print a buffer through a loop: {");
		i32 value;
		for_eachArr (value, buffer) { si_printf("%i ", value); }
		si_print("}\n");

		siDynamicArray(i32) array = si_dynamicArrayCopy(buffer, alloc);
		bool res = si_arrayEqual(SI_ARR_DYN(array), buffer);
		si_printfLn("\t(array == buffer) returns a '%t' boolean", res);

		i64* lastElement = si_dynamicArrayAppendItem(&array, 64, i64);
		si_printfLn("\tarray: \"%s\" (pointer to the element: %p)", si_dynamicArrayPrintInt(array), lastElement);

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
			"\tarray: '%s', len: '%zi'",
			si_stringFromArray(buffer, "%02#X", SI_ARR_STACK(256)),
			buffer.len
		);

		i32 value = INT32_MAX;
		isize posFirst = si_arrayFindItem(buffer, INT32_MAX, i32),
			  posLast = si_arrayFindLast(buffer, &value);
		isize occurences = si_arrayFindCount(buffer, SI_PTR(i32, INT32_MAX));

		si_printfLn(
			"\tThe integer '%02#X' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.",
			value, posFirst, posLast, occurences
		);

		isize pos = si_arrayFindItem(buffer, -123, i32);
		si_printfLn("\tHowever, the integer '-123' wasn't found and so, the function returns '%zi'", pos);
	}

	si_printLn("Scope 3:");
	{
		siDynamicArray(i32) array = si_dynamicArrayMake(alloc, i32, 1, 1, 2, 0, 6, 6, 6);

		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayReplaceAllItem(array, 6, 9, i32);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayEraseEx(&array, 1, 3);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayInsertArray(&array, 1, i32, 0xFF, 0xFFFF, 0x1991);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintInt(array));

		si_dynamicArrayEraseEx(&array, 1, 5);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintInt(array));
	}

	si_printLn("Scope 4:");
	{

		siDynamicArray(u32) array = si_dynamicArrayMake(alloc, siColor, {255, 0, 0, 255}, {128, 128, 128, 255}, {96, 255, 186, 255}, {23, 204, 2, 255});
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayReverse(array);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayClear(&array);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintClr(array));

		si_dynamicArrayFillItem(&array, 0, 4, SI_RGB(255, 255, 255), siColor);
		si_printfLn("\tarray: \"%s\"", si_dynamicArrayPrintClr(array));
	}
}
