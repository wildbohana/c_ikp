#pragma once
#include "Windows.h"

typedef HANDLE Heap;

typedef struct manager_struct {
	Heap* niz_heapova;				// Niz Heap-ova
	int* alocirani_bajtovi;			// Niz kolicine alocirane memorije
	int velicina_heapa;				// Maksimalna velicina "default" Heap-a
	int broj_heapova;				// Broj trenutno koriscenih Heap-ova
	int max_heapova;				// Maksimalan broj Heap-ova
	int trenutni_heap;				// Trenutni heap
	CRITICAL_SECTION manager_mutex; // Propusnica kriticne sekcije
} HeapManager;
