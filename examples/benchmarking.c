#define SI_IMPLEMENTATION 1
#include <sili.h>


void performance_test(void) {
	isize i;
	for (i = 0; i < UINT16_MAX; i++);
}


int main(void) {
	printf("Running 'performance_test()' 30000 times. Lets see how long it takes to execute that many times...\n");
	si_performance_run_per_loop(30000, performance_test());

	printf("Now lets see how many times 'performance_test()' can be executed in 5 seconds...\n");
	si_performance_executes_per_ms(5000, performance_test());

	printf("The average performance:\n");
	si_performance_loops_average(10000, performance_test());

	return 0;
}
