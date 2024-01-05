#include "ThreadTests.h"

CRITICAL_SECTION cs;
int counter = 0;

// Inicijalizacija AHM malloc-a
void ThreadTests_custom_malloc_initialize(int number_of_threads) {
	InitializeCriticalSection(&cs);

	// Dijagnostika
	clock_t start_time, end_time;
	double cpu_time_used;
	// Ako zelimo 4gb, samo stavimo 400000, ali moram ovako jer nemam dovoljno RAM-a
	int number_of_bytes = 200000 / number_of_threads;

	printf("\n\tAHM malloc i free funkcije\n");

	HANDLE* threads;
	threads = (HANDLE)malloc(number_of_threads * sizeof(HANDLE));

	// Stvaranje niti (sistemski poziv)
	// Odmah im zadajemo zadatak (ThreadTests_custom_malloc_and_custom_free)
	start_time = clock();
	for (int i = 0; i < number_of_threads; i++) {
		*(threads + i) = CreateThread(NULL, 0, &ThreadTests_custom_malloc_and_custom_free, (LPVOID)number_of_bytes, 0, NULL);
	}

	for (int i = 0; i < number_of_threads; i++) {
		WaitForSingleObject(*(threads + i), INFINITE);
	}
	end_time = clock();

	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("\nBroj tredova: %d\nPotrebno vreme za zauzimanje i oslobadjanje memorije: %f sekundi\n", number_of_threads, cpu_time_used);
	printf("\n----------------------------------------------------------------------\n");
	
	// Brisanje thread-ova
	for (int i = 0; i < number_of_threads; i++) {
		CloseHandle(*(threads + i));
	}
	free(threads);

	// Nije NULL treba da bude 200000 (2gb)
	printf("NIJE NULL: %d elemenata!", counter);
	counter = 0;
	DeleteCriticalSection(&cs);
	return 0;
}

// Inicijalizacija ugradjenog malloc-a
void ThreadTests_malloc_initialize(int number_of_threads) {
	// Dijagnostika
	clock_t start_time, end_time;
	double cpu_time_used;
	// Ako zelimo 4gb, stavimo 400000
	int number_of_bytes = 200000 / number_of_threads;

	HANDLE* threads;
	threads = (HANDLE)malloc(number_of_threads * sizeof(HANDLE));
	
	printf("\n\tUgradjene malloc i free funkcije\n");

	start_time = clock();
	for (int i = 0; i < number_of_threads; i++) {
		*(threads + i) = CreateThread(NULL, 0, &ThreadTests_malloc_and_free, (LPVOID)number_of_bytes, 0, NULL);
	}

	for (int i = 0; i < number_of_threads; i++) {
		WaitForSingleObject(*(threads + i), INFINITE);
	}
	end_time = clock();

	// Delta vremena
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("\nBroj tredova: %d\nPotrebno vreme za zauzimanje i oslobadjanje memorije: %f sekundi\n", number_of_threads, cpu_time_used);
	printf("\n----------------------------------------------------------------------\n");
	for (int i = 0; i < number_of_threads; i++) {
		CloseHandle(*(threads + i));
	}
	free(threads);
}

// TESTOVI AHM FUNKCIJA
DWORD WINAPI ThreadTests_custom_malloc_and_custom_free(LPVOID lpParam) {
	void* items[10000];
	int number_of_bytes = (int)lpParam;

	// malloc (10000 puta po 200mb = 2gb)
	for (int i = 0; i < 10000; i++) {
		items[i] = advanced_malloc(number_of_bytes);
	}

	// free
	for (int i = 0; i < 10000; i++) {
		
		if (items[i] != NULL)
		{
			advanced_free(items[i]);
			EnterCriticalSection(&cs);
				counter++;
			LeaveCriticalSection(&cs);
		}
	}

	return 0;
}

// TESTOVI UGRADJENIH FUNKCIJA
DWORD WINAPI ThreadTests_malloc_and_free(LPVOID lpParam) {
	void* items[10000];
	int number_of_bytes = (int)lpParam;

	// malloc
	for (int i = 0; i < 10000; i++) {
		items[i] = malloc(number_of_bytes);
	}

	// free
	for (int i = 0; i < 10000; i++) {
		if (items[i] != NULL)
			free(items[i]);
	}

	return 0;
}
