#define SI_IMPLEMENTATION 1
#include <sili.h>


b32 loopState = false;

/* A thread can only return a maximum of sizeof(rawptr) bytes, and take a sizeof(rawptr) byte parameter. */
i16 threadTest(b32* arg) {
	b32 loop = *arg;
	i16 count = INT16_MIN;

	if (loop) {
		printf("We'll increment 'count' from %d to %d:\n", INT16_MIN, INT16_MAX);
		si_sleep(2000);
		while (count < INT16_MAX) {
			count += 1;
			printf("%i\n", count);
		}
	}
	else {
		print("'arg' equals to 'false', so I'll just do nothing and wait for like 3 seconds.");
		si_sleep(3000);
		print("...and we're done! Exiting the thread now.");
	}

	return count;
}


int main(void) {
	siThread thread = si_threadCreate(threadTest, &loopState);
	si_threadStart(&thread);

	while (thread.isRunning) {
		print("Even though 'thread' is currently sleeping, it's still running this exact second!");
		si_sleep(1000);
	}

	printf("That loop returned a '%i'. Now we'll re-run the loop with the argument being 'true' instead.\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

    loopState = true;
	si_threadStart(&thread);
	si_threadJoin(&thread); /* Now we have to wait... */

	printf("That loop NOW returned a '%i'.\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

	#if !defined(SI_SYSTEM_WINDOWS) /* si_threadCancel is not supported on windows. Technically not good practice, not recommended. */
		si_threadStart(&thread);
		si_sleep(2500);
		si_threadCancel(&thread);

		print("Decided to kill it 2.5 seconds later.");
	#endif

	return 0;
}
