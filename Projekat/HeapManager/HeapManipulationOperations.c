#include "HeapManipulationOperations.h"

BOOL HeapManipulationOperations_get_heap(HeapManager* manager, Heap* out_heap) {
	BOOL ret = FALSE;

	if (manager != NULL && manager->heap_count > 0) {
		Heap temp;
		EnterCriticalSection(&manager->manager_mutex);

			// WORK IN PRPGRESS
			int minAllocIndex = 0;
			int minAllocSize = manager->heap_array[0];	// TODO dobavi kolicinu alocirane memorije
			for (int i = 1; i < manager->heap_count; i++)
			{
				if (minAllocSize < manager->heap_array[i])
				{
					minAllocSize = manager->heap_array[i];
					minAllocIndex = i;
				}
			}
			temp = manager->heap_array[minAllocIndex];

		LeaveCriticalSection(&manager->manager_mutex);
		if (temp != NULL) {
			*out_heap = temp;
			ret = TRUE;
		}
	}
	return ret;
}

// Nakon sto dobije Heap (od operacije iznad), alocira se memorija iz Heap-a
void* HeapManipulationOperations_get_memory(HeapManager* manager, int memory_size, Heap* out_heap) {
	if (manager != NULL && memory_size > 0) {
		Heap heap;
		if (HeapManipulationOperations_get_heap(manager, &heap)) {
			void* ret = HeapManipulation_allocate_memory(memory_size, heap);
			if (ret != NULL) {
				*out_heap = heap;
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
