#include "UnistiHeap.h"

// Unistavanje proslednjenog Heap-a
BOOL UnistiHeap_brisanje(Heap heap) {
	if (heap != NULL) {
		return HeapDestroy(heap);
	}
	else {
		return FALSE;
	}
}
