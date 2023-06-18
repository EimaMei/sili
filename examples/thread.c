#define SI_IMPLEMENTATION 1
#include <sili.h>


rawptr thread_test(bool* arg) {
	bool loop = *arg;
	i16 count = SI_INT16_MIN;

	if (loop) {
		printf("We'll increment 'count' from %d to %d:\n", SI_INT16_MIN, SI_INT16_MAX);
		si_sleep(2000);
		while (count < SI_INT16_MAX) {
			count += 1;
			printf("%i\n", count);
		}
	}
	else {
		printf("'arg' equals to 'false', so I'll just do nothing and wait for like 3 seconds.\n");
		si_sleep(3000);
		printf("...and we're done! Exiting the thread now.\n");
	}

	typeof(count)* res = malloc(sizeof(count));
	SI_ASSERT_NOT_NULL(res);
	*res = count;

	return res;
}


int main(void) {
	si_init(SI_KILO(1));

	siThread thread = si_thread_create(siFunc(thread_test), &(bool){false});
	si_thread_start(&thread);

	while (thread.is_running) {
		printf("Even though 'thread' is currently sleeping, it's still running this exact second!\n");
		si_sleep(1000);
	}

	printf("That loop returned a '%i'. Now we'll re-run the loop with the argument being 'true' instead.\n", *((i16*)thread.return_value));
	si_sleep(2000);

	thread.arg = &(bool){true};
	si_thread_start(&thread);
	si_thread_join(&thread); /* Now we have to wait... */

	printf("That loop NOW returned a '%i'.\n", *((i16*)thread.return_value));
	si_sleep(2000);

	#if !defined(SI_SYSTEM_WINDOWS)
		si_thread_start(&thread);
		si_sleep(2500);
		si_thread_cancel(&thread);

		printf("Decided to kill it 2.5 seconds later.\n");
	#endif

	si_terminate();
	return 0;
}
