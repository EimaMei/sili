#define SI_IMPLEMENTATION 1
#include <sili.h>


typedef struct matrixData {
	usize start;
	usize end;
	f32* a;
	f32* b;
	f32* result;
} matrixData;


i16 thread_test(b32* arg);
void thread_matrix(matrixData* data);


/* Example 1 shows off sili's threading functions and how they're meant to use
 * at their core. */
void example1(void);
/* Example 2 shows off how multi-threading can be used for parallel progrmaming
 * and how it can be much faster than a single-threaded function. */
void example2(void);


int main(void) {
	example1();
	example2();
}



void example1(void) {
	b32 loopState = false;
	siThread thread;
	si_threadMake(thread_test, &loopState, true, &thread);

	while (thread.isRunning) {
		si_print("Even though 'thread' is sleeping, the main thread is running independently.\n");
		si_sleep(1000);
	}
	si_printf("thread_test(false) returned a '%i'\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

	loopState = true;
	si_threadRun(&thread);
	si_threadJoin(&thread); /* Now we wait... */

	si_printf("thread_test(true) returned a '%i'.\n", si_threadGetReturn(thread, i16));
	si_sleep(2000);

	#if !defined(SI_SYSTEM_WINDOWS) /* si_threadCancel is not supported on windows. Not good practice either way, so it's not recommended. */
		si_threadRun(&thread);
		si_sleep(1000);
		si_threadCancel(&thread);

		si_print("Function got killed a second later.\n");
	#endif

	si_threadDestroy(&thread);
}


/* The maximum value of this is usually the amount of cores in your CPU. Going over
 * the core count doesn't resolve in better performance. */
#define THREAD_COUNT 4
/* The higher the number, the longer it takes. */
#define SIZE 400

void matrix_singlethreaded(f32* a, f32* b, f32* result);
void matrix_multithreaded(f32* a, f32* b, f32* result);


void example2(void) {
	siAllocator* alloc = si_allocatorMake(4 * (SIZE * SIZE * sizeof(f32)));

	/* Matrices A and B; res1 - single-threaded result, res2 - multi-threaded result. */
	f32* A = si_mallocArray(alloc, f32, SIZE * SIZE);
	f32* B = si_mallocArray(alloc, f32, SIZE * SIZE);
	f32* res1 = si_mallocArray(alloc, f32, SIZE * SIZE);
	f32* res2 = si_mallocArray(alloc, f32, SIZE * SIZE);


	/* Fill out both matrix A and B with random data. */
	srand((u32)si_clock());
	for_range (i, 0, SIZE) {
		for_range (j, 0, SIZE) {
			A[i * SIZE + j] = si_cast(f32, rand() % 10);
			B[i * SIZE + j] = si_cast(f32, rand() % 10);
		}
	}

	si_benchmarkLoopsAvgCmp(1, matrix_singlethreaded(A, B, res1), matrix_multithreaded(A, B, res2));

	/* Check if both outputs are correct. */
	for_range (i, 0, SIZE) {
		for_range (j, 0, SIZE) {
			SI_ASSERT_MSG(res1[i * SIZE + j] == res2[i * SIZE + j], "Results are incorrect!");
		}
	}
	si_printf("Results are correct.\n");

	si_allocatorFree(alloc);
}

void matrix_singlethreaded(f32* a, f32* b, f32* result) {
	for_range (i, 0, SIZE) {
		for_range (j, 0, SIZE) {
			for_range (k, 0, SIZE) {
				result[i * SIZE + j] = a[i * SIZE + k] * b[k * SIZE + j];
			}
		}
	}
}
void matrix_multithreaded(f32* a, f32* b, f32* result) {
	siThread threads[THREAD_COUNT];
	matrixData threadData[THREAD_COUNT];

	int rowsPerThread = SIZE / THREAD_COUNT;
	for_range (i, 0, THREAD_COUNT) {
		threadData[i].start = i * rowsPerThread;
		threadData[i].end = (i == THREAD_COUNT - 1)
			? SIZE
			: (i + 1) * rowsPerThread;

		threadData[i].a = a;
		threadData[i].b = b;
		threadData[i].result = result;

		si_threadMake(thread_matrix, &threadData[i], true, &threads[i]);
	}
	si_threadJoinArr(threads, countof(threads));

	for_range (i, 0, countof(threads)) {
		si_threadDestroy(&threads[i]);
	}
}



/* A thread can only return a maximum of 'sizeof(rawptr)' bytes, and take a
 * sizeof(rawptr) byte parameter. */
i16 thread_test(b32* arg) {
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

void thread_matrix(matrixData* data) {
	for_range (i, data->start, data->end) {
		for_range (j, 0, SIZE) {
			for_range (k, 0, SIZE) {
				data->result[i * SIZE + j] = data->a[i * SIZE + k] * data->b[k * SIZE + j];
			}
		}
	}
}
