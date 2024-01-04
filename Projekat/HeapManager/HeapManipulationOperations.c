#include "HeapManipulationOperations.h"

BOOL HeapManipulationOperations_get_heap(HeapManager* manager, Heap* out_heap) {
	BOOL ret = FALSE;

	if (manager != NULL && manager->heap_count > 0) {
		Heap temp;

		int minAllocIndex = 0;
		DWORD minAllocSize = 2 ^ 32;	// Umesto unsigned int

		EnterCriticalSection(&manager->manager_mutex);

			HANDLE hHeap = manager->heap_array[0];
			PROCESS_HEAP_ENTRY Entry;
			DWORD tempSize = 0;

			// Prolaz kroz sve Heap-ove
			for (int i = 0; i < manager->heap_count; i++) {
				// Inicijalizacija HeapWalk-a
				Entry.lpData = NULL;
				while (HeapWalk(hHeap, &Entry) != FALSE) {
					if ((Entry.wFlags & PROCESS_HEAP_REGION) != 0) {
						tempSize = Entry.Region.dwCommittedSize;
						// Provera da li je manji od prethodnog
						if (tempSize < minAllocSize)
						{
							minAllocSize = tempSize;
							minAllocIndex = i;
						}
					}
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
