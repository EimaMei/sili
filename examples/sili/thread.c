#define SI_IMPLEMENTATION 1
#include <sili.h>


SIDEF SI_THREAD_PROC(thread_test);
SIDEF SI_THREAD_PROC(thread_matrix);

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
	bool loopState = false;

	siThread thread;
	si_threadMakeAndRun(thread_test, &loopState, &thread);

	while (thread.state == siThreadState_Running) {
		si_print("Even though 'thread' is sleeping, the main thread is running independently.\n");
		si_sleep(SI_TIME_S(1));
	}
	si_printfLn("thread_test(false) returned a '%i'", si_threadGetReturn(thread, i16));
	si_sleep(SI_TIME_S(2));

	loopState = true;
	si_threadRun(&thread);
	si_threadJoin(&thread); /* Now we wait... */

	si_printfLn("thread_test(true) returned a '%i'", si_threadGetReturn(thread, i16));
	si_threadDestroy(&thread);
}


/* The maximum value of this is usually the amount of cores in your CPU. Going over
 * the core count doesn't resolve in better performance. */
#define THREAD_COUNT 4
/* The higher the number, the longer it takes. */
#define SIZE 64

void matrix_singlethreaded(f32* a, f32* b, f32* result);
void matrix_multithreaded(f32* a, f32* b, f32* result);


typedef struct matrixData {
	isize start;
	isize end;
	f32* a;
	f32* b;
	f32* result;
} matrixData;


void example2(void) {
	siArena aData = si_arenaMake(si_allocatorHeap(), 4 * (SIZE * SIZE * sizeof(f32)));
	siAllocator alloc = si_allocatorArena(&aData);

	/* Matrices A and B; res1 - single-threaded result, res2 - multi-threaded result. */
	f32* A = si_allocArrayNonZeroed(alloc, f32, SIZE * SIZE);
	f32* B = si_allocArrayNonZeroed(alloc, f32, SIZE * SIZE);
	f32* res1 = si_allocArrayNonZeroed(alloc, f32, SIZE * SIZE);
	f32* res2 = si_allocArrayNonZeroed(alloc, f32, SIZE * SIZE);


	/* Fill out both matrix A and B with random data. */
	srand((u32)(si_clock() / SI_MILLISECOND));
	for_range (i, 0, SIZE) {
		for_range (j, 0, SIZE) {
			A[i * SIZE + j] = (f32)(rand() % 10);
			B[i * SIZE + j] = (f32)(rand() % 10);
		}
	}

	si_benchmarkLoopsAvgCmp(1000, matrix_singlethreaded(A, B, res1), matrix_multithreaded(A, B, res2));

	/* Check if both outputs are correct. */
	for_range (i, 0, SIZE) {
		for_range (j, 0, SIZE) {
			SI_ASSERT_MSG(res1[i * SIZE + j] == res2[i * SIZE + j], "Results are incorrect!");
		}
	}
	si_printLn("Results are correct.");

	si_arenaFree(&aData);
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
	matrixData data[THREAD_COUNT];

	i32 rows = SIZE / THREAD_COUNT;
	for_range (i, 0, THREAD_COUNT) {
		data[i].start = i * rows;
		data[i].end = (i == THREAD_COUNT - 1)
			? SIZE
			: (i + 1) * rows;

		data[i].a = a;
		data[i].b = b;
		data[i].result = result;

		si_threadMakeAndRun(thread_matrix, &data[i], &threads[i]);
	}
	for_range (i, 0, THREAD_COUNT) { si_threadJoin(&threads[i]); }
	for_range (i, 0, THREAD_COUNT) { si_threadDestroy(&threads[i]); }
}



/* A thread can only return a maximum of 'sizeof(void*)' bytes, and take a
 * sizeof(void*) byte parameter. */
void* thread_test(void* arg) {
	bool loop = *(bool*)arg;
	i16 count = INT16_MIN;

	if (loop) {
		si_printfLn("The function will increment 'count' from %d to %d:", INT16_MIN, INT16_MAX);
		si_sleep(SI_TIME_S(2));
		while (count < INT16_MAX) {
			count += 1;
		}
	}
	else {
		si_printLn("'arg' equals to 'false', so the function will do nothing and sleep for 3 seconds.");
		si_sleep(SI_TIME_S(3));
		si_printLn("Exiting the thread now.");
	}

	return si_transmute(void*, count, i16);
}

void* thread_matrix(void* mData) {
	matrixData* data = (matrixData*)mData;

	for_range (i, data->start, data->end) {
		for_range (j, 0, SIZE) {
			for_range (k, 0, SIZE) {
				data->result[i * SIZE + j] = data->a[i * SIZE + k] * data->b[k * SIZE + j];
			}
		}
	}

	return nil;
}
