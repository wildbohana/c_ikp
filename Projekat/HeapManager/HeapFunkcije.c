#include "HeapFunkcije.h"

// Alociranje memorije iz proslednjenog Heap-a
void* HeapFunkcije_alociraj(int bajta, Heap heap) {
	if (bajta > 0 && heap != NULL) {
		return HeapAlloc(heap, 0, bajta);
	}
	else {
		return NULL;
	}
}

// Alociranje memorije iz prosledjenog Heap-a
// Nije thread-safe
void* HeapFunkcije_alociraj_unsafe(int bajta, Heap heap) {
	if (bajta > 0 && heap != NULL) {
		return HeapAlloc(heap, HEAP_NO_SERIALIZE, bajta);
	}
	else {
		return NULL;
	}
}

// Oslobadjanje memorije iz proslednjenog Heap-a
BOOL HeapFunkcije_dealociraj(void* pokazivac, Heap heap) {
	if (heap != NULL && pokazivac != NULL) {
		return HeapFree(heap, 0, pokazivac);
	}
	else {
		return FALSE;
	}
}

// Oslobadjanje memorije iz prosledjenog Heap-a
// Nije thread-safe
BOOL HeapFunkcije_dealociraj_unsafe(void* pokazivac, Heap heap) {
	if (heap != NULL && pokazivac != NULL) {
		return HeapFree(heap, HEAP_NO_SERIALIZE, pokazivac);
	}
	else {
		return FALSE;
	}
}
