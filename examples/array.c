#define SI_IMPLEMENTATION 1
#include <sili.h>


void example1(siAllocator* heap) {
    si_allocatorReset(heap);
	printf("==============\n\n==============\nExample 1:\n");

	siArray(i32) array = si_arrayMake(heap, (i32[]){3, 234, 2, 4, 294, 234, 23});

    print("All of the elements in 'array':");
	for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: %i\n", i, array[i]);
	}

	isize find_pos = si_arrayFind(array, 234);
	isize rfind_pos = si_arrayRFind(array, 234);
	printf("The 1st number '234' is at 'array[%zd]', while the 2nd one is at 'array[%zd]'\n", find_pos, rfind_pos);

	usize previous_len = si_arrayLen(array);
	si_arrayAppend(&array, INT32_MAX); /* si_array_push does the same thing. */

	i32* front = si_arrayFront(array);
	i32* back = si_arrayBack(array);
	printf("We now have %zd elements instead of %zd. The front value is '%i', while the back value is '0x%X'\n", si_arrayLen(array), previous_len, *front, *back);

	si_arrayReplace(&array, 4, INT32_MIN);
	printf("The element at position '%d' was replaced with: -'%i'\n", 3, array[3]);

	siArray(i32) copy = si_arrayCopy(heap, array);
	b32 res = si_arrayEqual(array, copy);

	printf("Arrays 'array' and 'copy' are %s\n", (res ? "the same" : "NOT the same"));
}

void example2(siAllocator* heap) {
    si_allocatorReset(heap);
	printf("==============\n\n==============\nExample 2:\n");

	siArray(i32) array = si_arrayMake(
        heap,
        si_buf(i32, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    );

	printf("Array in regular order: ");
	for_each (num, array) {
		printf("%i ", *num);
	}
	printf("\n");


	si_arrayReverse(array);

	printf("Array in reverse order: ");
	for_range (num, 0, si_arrayLen(array)) {
		printf("%i ", array[num]);
	}
	printf("\n");
}


void example3(siAllocator* heap) {
    si_allocatorReset(heap);
    printf("==============\n\n==============\nExample 3:\n");

    siArray(siColor) array = si_arrayMake(
        heap,
        si_buf(siColor, SI_RGB(255, 0, 0), SI_RGBA(0, 255, 0, 127), SI_RGB(0, 0, 255))
    );

    si_arrayAppend(&array, (siColor){255, 255, 255, 255});

    printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
    for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_arrayPop(&array);
    printf("Current length now - '%zd'\n", si_arrayLen(array));


    si_arrayInsert(&array, SI_RGB(127, 127, 127), 2);

    printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
    for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_arrayErase(&array, 2);

    printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
    for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_arrayEraseCount(&array, 0, 3);
    printf("array_empty: '%u', capacity: '%zd'\n", si_arrayEmpty(array), si_arrayCapacity(array));

    si_arrayFill(&array, 0, si_arrayCapacity(array), SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));

    printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
    for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_arrayRemoveItem(&array, SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));
    printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
}


int main(void) {
    siAllocator* heap = si_allocatorMake(0xFF);

    example1(heap);
	example2(heap);
    example3(heap);

	si_allocatorFree(heap);
	return 0;
}
