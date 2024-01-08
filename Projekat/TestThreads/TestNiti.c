#include "TestNiti.h"

CRITICAL_SECTION cs;
int brojac = 0;

// Inicijalizacija AHM testova
void test_ahm_init(int broj_niti) {
	InitializeCriticalSection(&cs);

	// 400000 * 10000 = 4gb (10000 alloc iz testa)
	int broj_bajta = 400000 / broj_niti;

	printf("\n\tAHM malloc i free funkcije\n");

	HANDLE* niti;
	niti = (HANDLE)malloc(broj_niti * sizeof(HANDLE));

	// Dijagnostika
	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	// Stvaranje niti (sistemski poziv) za zadatkom (AHM operacije)
	for (int i = 0; i < broj_niti; i++) {
		*(niti + i) = CreateThread(NULL, 0, &test_ahm_funkcije, (LPVOID)broj_bajta, 0, NULL);
	}

	for (int i = 0; i < broj_niti; i++) {
		WaitForSingleObject(*(niti + i), INFINITE);
	}
	end_time = clock();

	// Racunanje delte
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("\nBroj niti: %d\nPotrebno vreme za zauzimanje i oslobadjanje memorije: %f sekundi\n", broj_niti, cpu_time_used);
	printf("\n----------------------------------------------------------------------\n");
	
	// Brisanje niti
	for (int i = 0; i < broj_niti; i++) {
		CloseHandle(*(niti + i));
	}
	free(niti);

	// Nije NULL treba da bude 400000 (4gb)
	printf("NIJE NULL (uspesna alokacija): %d elemenata!\n", brojac);
	brojac = 0;
	DeleteCriticalSection(&cs);
	return 0;
}

// Inicijalizacija obicnih testova
void test_obican_init(int broj_niti) {
	// 400000 * 10000 = 4gb (10000 alloc iz testa)
	int broj_bajta = 400000 / broj_niti;

	HANDLE* niti;
	niti = (HANDLE)malloc(broj_niti * sizeof(HANDLE));
	
	printf("\n\tUgradjene malloc i free funkcije\n");
	
	// Dijagnostika
	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	// Stvaranje niti (sistemski poziv) za zadatkom (ugradjene operacije)
	for (int i = 0; i < broj_niti; i++) {
		*(niti + i) = CreateThread(NULL, 0, &test_ugradjene_funkcije, (LPVOID)broj_bajta, 0, NULL);
	}

	for (int i = 0; i < broj_niti; i++) {
		WaitForSingleObject(*(niti + i), INFINITE);
	}
	end_time = clock();

	// Racunanje delte
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("\nBroj niti: %d\nPotrebno vreme za zauzimanje i oslobadjanje memorije: %f sekundi\n", broj_niti, cpu_time_used);
	printf("\n----------------------------------------------------------------------\n");

	// Brisanje niti
	for (int i = 0; i < broj_niti; i++) {
		CloseHandle(*(niti + i));
	}
	free(niti);
}

// TESTOVI AHM FUNKCIJA
DWORD WINAPI test_ahm_funkcije(LPVOID lpParam) {
	void* objekti[10000];
	int broj_bajta = (int)lpParam;

	// ahm malloc (10000 puta po 400kb = 4gb)
	for (int i = 0; i < 10000; i++) {
		objekti[i] = ahm_malloc(broj_bajta);
	}

	// ahm free
	for (int i = 0; i < 10000; i++) {
		
		if (objekti[i] != NULL)
		{
			ahm_free(objekti[i]);
			EnterCriticalSection(&cs);
				brojac++;
			LeaveCriticalSection(&cs);
		}
	}

	return 0;
}

// TESTOVI UGRADJENIH FUNKCIJA
DWORD WINAPI test_ugradjene_funkcije(LPVOID lpParam) {
	void* objekti[10000];
	int broj_bajta = (int)lpParam;

	// ugradjen malloc
	for (int i = 0; i < 10000; i++) {
		objekti[i] = malloc(broj_bajta);
	}

	// ugradjen free
	for (int i = 0; i < 10000; i++) {
		if (objekti[i] != NULL) {
			free(objekti[i]);
		}
	}

	return 0;
}
