#include "HeapManipulationOperations.h"

BOOL HeapManipulationOperations_get_heap(HeapManager* manager, int* out_heap_idx) {
	BOOL ret = FALSE;

	int minAllocIndex = manager->current_heap;
	int minAllocSize = manager->heap_allocated_bytes[minAllocIndex];

	if (manager != NULL && manager->heap_count > 0) {
		
		EnterCriticalSection(&manager->manager_mutex);
			// Prolaz kroz sve Heap-ove, traži se onaj sa najmanje alociranih bajta
			for (int i = 0; i < manager->heap_count; i++) {
				if (manager->heap_allocated_bytes[i] != -1 && manager->heap_allocated_bytes[i] <= minAllocSize) {
					minAllocIndex = i;
					minAllocSize = manager->heap_allocated_bytes[i];
				}
			}
		LeaveCriticalSection(&manager->manager_mutex);
		
		if (minAllocIndex != -1) {
			*out_heap_idx = minAllocIndex;
			manager->current_heap = minAllocIndex;
			ret = TRUE;
		}
	}
	return ret;
}

// Nakon sto dobije Heap (od operacije iznad), alocira se memorija iz Heap-a
void* HeapManipulationOperations_get_memory(HeapManager* manager, int memory_size, Heap* out_heap) {
	if (manager != NULL && memory_size > 0) {
		Heap heap;
		int heap_idx;

		if (HeapManipulationOperations_get_heap(manager, &heap_idx)) {
			heap = manager->heap_array[heap_idx];
			void* ret = HeapManipulation_allocate_memory(memory_size, heap);
			if (ret != NULL) {
				*out_heap = heap;
				manager->heap_allocated_bytes[heap_idx] += memory_size;
				return ret;
			}
			else {
				return NULL;
			}
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

// Oslobadjanje memorije iz Heap-a
BOOL HeapManipulationOperations_free_memory(HeapManager* manager, void* pointer, Heap heap) {
	if (manager != NULL && pointer != NULL && heap != NULL) {
		return HeapManipulation_free_memory(pointer, heap);
	}
	else {
		return FALSE;
	}
}
