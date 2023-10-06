#define SI_IMPLEMENTATION 1
#include <sili.h>


int main(void) {
	siAllocator* stack = si_allocator_make_stack(0xFF); /* Functions that'll allocate to this allocator will only do it in the stack, not the heap! */

	/* Example 3.0: siPair (based on https://cplusplus.com/reference/utility/pair/pair/) */
	printf("==============\n\n==============\nExample 3.0:\n");

	siPair(siString, f64) product1;
  	siPair(siString, f64) product2 = si_pair_make(si_string_make(stack, "tomatoes"), 2.30);
  	siPair(siString, f64) product3 = si_pair_copy(product2);
  	siPair(siString, f64) product4 = {si_string_make(stack, "bread"), 599.99};

	product1.first = si_string_make(stack, "shoes");
	product1.second = 39.90;

	printf("The price of %s is $%f\n", product1.first, product1.second);
	printf("The price of %s is $%f\n", product2.first, product2.second);
	printf("The price of %s is $%f\n", product3.first, product3.second);
	printf("The price of %s is $%f\n", product4.first, product4.second);

	return 0;
}
