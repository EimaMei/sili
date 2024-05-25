#define SI_IMPLEMENTATION 1
#include <sili.h>


int main(void) {
	siAllocator* stack = si_allocatorMakeStack(0xFF);

	/* Example 3.0: siPair (based on https://cplusplus.com/reference/utility/pair/pair/) */
	si_printf("==============\n\n==============\nExample 3.0:\n");

	siPair(siString, f64) product1;
	siPair(siString, f64) product2 = si_pairMake(si_stringMake(stack, "tomatoes"), 2.30);
	siPair(siString, f64) product3 = si_pairCopy(product2);
	siPair(siString, f64) product4 = {si_stringMake(stack, "bread"), 599.99};

	product1.first = si_stringMake(stack, "shoes");
	product1.second = 39.90;

	si_printf("The price of %s is $%.2f\n", product1.first, product1.second);
	si_printf("The price of %s is $%.2f\n", product2.first, product2.second);
	si_printf("The price of %s is $%.2f\n", product3.first, product3.second);
	si_printf("The price of %s is $%.2f\n", product4.first, product4.second);

	return 0;
}
