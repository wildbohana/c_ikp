#include "HeapManipulation.h"

// Alociranje memorije iz proslednjenog Heap-a
void* HeapManipulation_allocate_memory(int bytes, Heap heap) {
	if (bytes > 0 && heap != NULL) {
		return HeapAlloc(heap, 0, bytes);
	}
	else {
		return NULL;
	}
}

// Alociranje memorije iz prosledjenog Heap-a
// Nije thread-safe
void* HeapManipulation_allocate_memory_unlocked(int bytes, Heap heap) {
	if (bytes > 0 && heap != NULL) {
		return HeapAlloc(heap, HEAP_NO_SERIALIZE, bytes);
	}
	else {
		return NULL;
	}
}

// Oslobadjanje memorije iz proslednjenog Heap-a
BOOL HeapManipulation_free_memory(void* pointer, Heap heap) {
	if (heap != NULL && pointer != NULL) {
		return HeapFree(heap, 0, pointer);
	}
	else {
		return FALSE;
	}
}

// Oslobadjanje memorije iz prosledjenog Heap-a
// Nije thread-safe
BOOL HeapManipulation_free_memory_unlocked(void* pointer, Heap heap) {
	if (heap != NULL && pointer != NULL) {
		return HeapFree(heap, HEAP_NO_SERIALIZE, pointer);
	}
	else {
		return FALSE;
	}
}
