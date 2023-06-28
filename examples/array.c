#define SI_ALLOCATOR_UNDEFINE 1
#define SI_IMPLEMENTATION 1
#include <sili.h>


void example_2_0(void) {
	printf("==============\n\n==============\nExample 2.0:\n");

	siArray(i32) array = si_array_make((i32[]){3, 234, 2, 4, 294, 234, 23});

    print("All of the elements in 'array':");
	for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: %i\n", i, array[i]);
	}

	isize find_pos = si_array_find(array, 234);
	isize rfind_pos = si_array_rfind(array, 234);
	printf("The 1st number '234' is at 'array[%zd]', while the 2nd one is at 'array[%zd]'\n", find_pos, rfind_pos);

	usize previous_len = si_array_len(array);
	si_array_append(&array, INT32_MAX); /* si_array_push_back does the same thing. */

	i32 front = si_any_get(i32, si_array_front(array));
	i32 back = si_any_get(i32, si_array_back(array));
	printf("We now have %zd elements instead of %zd. The front value is '%i', while the back value is '0x%X'\n", si_array_len(array), previous_len, front, back);

	si_array_replace(&array, 4, INT32_MIN);
	printf("The element at position '%d' was replaced with: -'-0x%X'\n", 3, array[3]);

	siArray(i32) copy = si_array_copy(array);
	bool res = si_array_equal(array, copy);

	printf("Arrays 'array' and 'copy' are %s\n", (res ? "the same" : "NOT the same"));
}

void example_2_1(void) {
	printf("==============\n\n==============\nExample 2.1:\n");


	siArray(i32) array = si_array_make((i32[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	printf("Array in regular order: ");
	foreach (num, array) {
		printf("%i ", *num);
	}
	printf("\n");


	si_array_reverse(&array);

	printf("Array in reverse order: ");
	for_range (num, 0, si_array_len(array)) {
		printf("%i ", array[num]);
	}
	printf("\n");
}


void example_2_2(void) {
    siArray(siColor) array = si_array_make((siColor[]){SI_RGB(255, 0, 0), SI_RGBA(0, 255, 0, 127), SI_RGB(0, 0, 255)});

    si_array_append(&array, (siColor){255, 255, 255, 255});

    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
    for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_array_pop(&array);
    printf("Current length now - '%zd'\n", si_array_len(array));


    si_array_insert(&array, SI_RGB(127, 127, 127), 2);

    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
    for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_array_erase(&array, 2);

    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
    for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_array_erase_count(&array, 0, 3);
    printf("array_empty: '%zd', capacity: '%zd'\n", si_array_empty(array), si_array_capacity(array));

    si_array_fill(&array, 0, si_array_capacity(array), SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));

    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
    for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: (%i, %i, %i, %i)\n", i, array[i].r, array[i].g, array[i].g, array[i].b);
	}

    si_array_remove_item(&array, SI_RGBA(0xFF, 0xFF, 0xFF, 0xFF));
    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
}


int main(void) {
	si_init(SI_KILO(1));

	example_2_0();
	example_2_1();
    example_2_2();

	si_terminate();
	return 0;
}
