#define SI_IMPLEMENTATION 1
#include <sili.h>


b32 loopState = false;

/* A thread can only return a maximum of sizeof(rawptr) bytes, and take a sizeof(rawptr) byte parameter. */
i16 threadTest(b32* arg) {
	b32 loop = *arg;
	i16 count = INT16_MIN;

	if (loop) {
		si_printf("The function will increment 'count' from %d to %d:\n", INT16_MIN, INT16_MAX);
		si_sleep(2000);
		while (count < INT16_MAX) {
			count += 1;
		}
	}
	else {
		si_print("'arg' equals to 'false', so the function will do nothing and sleep for 3 seconds.\n");
		si_sleep(3000);
		si_print("Exiting the thread now.\n");
	}

	return count;
}


int main(void) {
	siThread thread = si_threadCreate(threadTest, &loopState);
	si_threadStart(&thread);

	while (thread.isRunning) {
		si_print("Even though 'thread' is sleeping, the main thread is running independent of it.\n");
		si_sleep(1000);
	}

	si_printf("threadTest(false) returned a '%i'\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

	loopState = true;
	si_threadStart(&thread);
	si_threadJoin(&thread); /* Now we have to wait. */

	si_printf("threadTest(true) returned a '%i'.\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

	#if !defined(SI_SYSTEM_WINDOWS) /* si_threadCancel is not supported on windows. Not good practice either way, so it's not recommended. */
		si_threadStart(&thread);
		si_sleep(2500);
		si_threadCancel(&thread);

		si_print("Function got killed 2.5 seconds later.\n");
	#endif

	return 0;
}
