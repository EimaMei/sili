#define SI_IMPLEMENTATION 1
#include <sili.h>


/* Prints the contents of an i32-typed array. */
siString print_arr_int(siArray(i32) arr);
/* Prints the contents of a siColor-typed dynamic array. */
siString print_arr_clr(siDynamicArray(siColor) arr);


int main(void) {
	siArena arena = si_arenaMakePtr(si_stackAlloc(256));
	siAllocator alloc = si_allocatorArena(&arena);

	si_printLn("Scope 1:");
	{
		siArray(i32) buffer = SI_ARR(i32, 1, 2, 4, 8, 16, 32);
		si_printfLn("\tbuffer: \"%s\"", print_arr_int(buffer));

		si_print("\tyou can also print a buffer through a loop: {");
		i32 value;
		for_eachArr (value, buffer) { si_printf("%i ", value); }
		si_print("}\n");

		siDynamicArray(i32) array = si_dynamicArrayCopy(buffer, alloc);
		bool res = si_arrayEqual(array.arr, buffer);
		si_printfLn("\t(array == buffer) returns a '%t' boolean", res);

		bool realloced = si_dynamicArrayAppend(&array, 64);
		si_printfLn("\tarray: \"%s\" (reallocated: %t)", print_arr_int(array.arr), realloced);

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
			"\tarray: '%s', len: '%i'",
			si_stringFromArray(SI_ARR_STACK(256), buffer, SI_STR("%02#X"), si_typeid(i32)),
			buffer.len
		);

		isize posFirst = si_arrayFind(buffer, INT32_MAX),
			  posLast = si_arrayFindLast(buffer, INT32_MAX);
		isize occurences = si_arrayFindCount(buffer, INT32_MAX);

		si_printfLn(
			"\tThe integer '%02#X' was first found at index '%zi', last found at index '%zi', with '%zi' occurences in total.",
			INT32_MAX, posFirst, posLast, occurences
		);

		isize pos = si_arrayFind(buffer, -123);
		si_printfLn("\tHowever, the integer '-123' wasn't found and so, the function returns '%zi'", pos);
	}

	si_printLn("Scope 3:");
	{
		siDynamicArray(i32) array = si_dynamicArrayMake(alloc, i32, 1, 1, 2, 0, 6, 6, 6);

		si_printfLn("\tarray: \"%s\"", print_arr_int(array.arr));

		si_dynamicArrayReplaceAll(array, 6, 9);
		si_printfLn("\tarray: \"%s\"", print_arr_int(array.arr));

		si_dynamicArrayErase(&array, 1, 3);
		si_printfLn("\tarray: \"%s\"", print_arr_int(array.arr));

		si_dynamicArrayInsert(&array, 1, 0xFF, 0xFFFF, 0x1991);
		si_printfLn("\tarray: \"%s\"", print_arr_int(array.arr));

		si_dynamicArrayErase(&array, 1, 5);
		si_printfLn("\tarray: \"%s\"", print_arr_int(array.arr));
	}

	si_printLn("Scope 4:");
	{
		typedef struct { u8 r, g, b, a; } siColor;

		siDynamicArray(u32) array = si_dynamicArrayMake(
			alloc, siColor,
			{255,   0,   0, 255}, {128, 128, 128, 255},
			{ 96, 255, 186, 255}, { 23, 204,   2, 255}
		);
		si_printfLn("\tarray: \"%s\"", print_arr_clr(array));

		si_dynamicArrayReverse(array);
		si_printfLn("\tarray: \"%s\"", print_arr_clr(array));

		si_dynamicArrayClear(&array);
		si_printfLn("\tarray: \"%s\"", print_arr_clr(array));

		si_dynamicArrayFill(&array, 0, 4, SI_COMP_LIT(siColor, 255, 255, 255, 255));
		si_printfLn("\tarray: \"%s\"", print_arr_clr(array));
	}
}


siString print_arr_int(siArray(i32) arr) {
	static u8 static_array[256];
	return si_stringFromArray(SI_ARR_BUF(static_array), arr, SI_STR("%i"), si_typeid(i32));
}

siString print_arr_clr(siDynamicArray(siColor) arr) {
	static u8 static_array[256];
	siTypeId id_u8 = si_typeid(u8);
	return si_stringFromArray(
		SI_ARR_BUF(static_array), arr.arr, SI_STR("siColor{%i, %i, %i}"), id_u8, id_u8, id_u8
	);
}
