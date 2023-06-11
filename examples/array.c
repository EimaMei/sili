//#define SI_IMPLEMENTATION 1
#include <sili.h>


void example_2_0(void) {
    printf("==============\n\n==============\nExample 2.0:\n");

	siArray(i32) array = si_array_make((i32[]){3, 234, 2, 4, 294, 234, 23});

	foreach (num, array) {
		printf("Element %zd: %i\n", (num - array), *num);
	}

	isize find_pos = si_array_find(array, 234);
	isize rfind_pos = si_array_rfind(array, 234);
	printf("The 1st number '234' is at 'array[%zd]', while the 2nd one is at 'array[%zd]'\n", find_pos, rfind_pos);

	usize previous_len = si_array_len(array);
	si_array_append(&array, SI_INT32_MAX); /* si_array_push_back does the same thing. */

	i32 front = si_any_get(si_array_front(array), i32);
	i32 back = si_any_get(si_array_back(array), i32);
	printf("We now have %zd elements instead of %zd. The front value is '%i', while the back value is '0x%X'\n", si_array_len(array), previous_len, front, back);

	si_array_replace(&array, 4, SI_INT32_MIN);
	printf("The element at position '%d' was replaced with: -'-0x%X'\n", 3, array[3]);

	siArray(i32) copy = si_array_copy(array);
	bool res = si_arrays_are_equal(array, copy);

	printf("Arrays 'array' and 'copy' are %s\n", (res ? "the same" : "NOT the same"));

	si_array_free(array);
	si_array_free(copy);
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
	foreach (num, array) {
		printf("%i ", *num);
	}
	printf("\n");
	si_array_free(array);
}


int main(void) {
    example_2_0();
    example_2_1();

    return 0;
}
