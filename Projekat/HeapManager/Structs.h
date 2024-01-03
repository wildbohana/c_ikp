#pragma once
#include "Windows.h"

typedef HANDLE Heap;

typedef struct manager_struct {
	Heap* heap_array;				// Niz Heap-ova
	int heap_size;					// Maksimalna velicina "default" Heap-a
	int heap_count;					// Broj trenutno koriscenih Heap-ova
	int max_heaps;					// Maksimalan broj Heap-ova
	CRITICAL_SECTION manager_mutex; // Propusnica kriticne sekcije
} HeapManager;
