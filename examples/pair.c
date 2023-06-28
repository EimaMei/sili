#define SI_IMPLEMENTATION 1
#include <sili.h>


int main(void) {
	si_init(SI_KILO(1));

	/* Example 3.0: siPair (based on https://cplusplus.com/reference/utility/pair/pair/) */
	printf("==============\n\n==============\nExample 3.0:\n");

	siPair(siString, f64) product1;
  	siPair(siString, f64) product2 = si_pair_make(si_string_make("tomatoes"), 2.30);
  	siPair(siString, f64) product3 = si_pair_copy(product2);
  	siPair(siString, f64) product4 = {si_string_make("bread"), 599.99};

	product1.first = si_string_make("shoes");
	product1.second = 39.90;

	printf("The price of %s is $%f\n", product1.first, product1.second);
	printf("The price of %s is $%f\n", product2.first, product2.second);
	printf("The price of %s is $%f\n", product3.first, product3.second);
	printf("The price of %s is $%f\n", product4.first, product4.second);

	si_terminate();
	return 0;
}
