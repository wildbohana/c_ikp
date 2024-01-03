#include "HeapDestruction.h"

// Unistavanje proslednjenog Heap-a
BOOL HeapDestruction_destroy_heap(Heap heap) {
	if (heap != NULL) {
		return HeapDestroy(heap);
	}
	else {
		return FALSE;
	}
}
