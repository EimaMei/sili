#define SI_IMPLEMENTATION 1
#include <sili.h>


void print_arrI(siArray(void) arr) {
	usize len = 0;
	char buf[SI_KILO(4)];

	buf[0] = '{', len += 1;
	for_range (i, 0, si_arrayLen(arr) - 1) {
		len += si_snprintf(&buf[len], sizeof(buf) - len, "%i, ", ((i32*)arr)[i]) - 1;
	}
	si_snprintf(&buf[len], sizeof(buf) - len, "%i}\n", ((i32*)arr)[si_arrayLen(arr)]);

	si_printf(buf);
}

void example1(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\n\n==============\nExample 1:\n");

	siArray(i32) array = si_arrayMake(heap, (i32[]){3, 234, 2, 4, 294, 234, 23});

	si_print("All of the elements in 'array': "); print_arrI(array);

	siOptional(usize) find_pos = si_arrayFind(array, i32, 234);
	siOptional(usize) rfind_pos = si_arrayRFind(array, i32, 234);
	si_printf(
		"The 1st number '234' is at 'array[%zu]', while the 2nd one is at 'array[%zu]'\n",
		find_pos->value, rfind_pos->value
	);

	usize previousLen = si_arrayLen(array);
	si_arrayAppend(&array, i32, INT32_MAX); /* si_array_push does the same thing. */

	i32* front = si_arrayFront(array);
	i32* back = si_arrayBack(array);
	si_printf(
		"We now have %zd elements instead of %zd. The front value is '%i', while the back value is '0x%X'\n",
		si_arrayLen(array), previousLen, *front, *back
	);

	si_arrayReplace(&array, i32, 4, INT32_MIN);
	si_printf("The element at position '%d' was replaced with: -'%i'\n", 3, array[3]);

	siArray(i32) copy = si_arrayCopy(heap, array);
	b32 res = si_arrayEqual(array, copy);

	si_printf("Arrays 'array' and 'copy' are the same: %B\n", res);
}

void example2(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\n\n==============\nExample 2:\n");

	siArray(u32) array = si_arrayMake(
		heap,
		si_buf(u32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
	);

	si_print("Array in regular order: "); print_arrI(array);
	si_arrayReverse(array);
	si_print("Array in reverse order: "); print_arrI(array);
}


void example3(siAllocator* heap) {
	si_allocatorReset(heap);
	si_printf("==============\n\n==============\nExample 3:\n");

	siArray(siColor) array = si_arrayMake(
		heap,
		si_buf(siColor, SI_RGB(255, 0, 0), SI_RGBA(0, 255, 0, 127), SI_RGB(0, 0, 255))
	);

	si_arrayAppend(&array, siColor, {255, 255, 255, 255});

	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		si_printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

	si_arrayPop(&array);
	si_printf("Current length now - '%zd'\n", si_arrayLen(array));

	si_arrayInsert(&array, siColor, 2, SI_RGB(127, 127, 127));

	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		si_printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

	si_arrayErase(&array, 2);

	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		si_printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

	si_arrayEraseCount(&array, 0, 3);
	si_printf("array_empty: '%u', capacity: '%zd'\n", si_arrayEmpty(array), si_arrayCapacity(array));

	si_arrayFill(&array, siColor, 0, si_arrayCapacity(array), SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));

	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		si_printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

	si_arrayRemoveItem(&array, siColor, SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));
	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
}


int main(void) {
	siAllocator* heap = si_allocatorMake(0xFF);

	example1(heap);
	example2(heap);
	example3(heap);

	si_allocatorFree(heap);
	return 0;
}
