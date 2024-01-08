#include "HeapOperacije.h"

BOOL HeapOperacije_dobavi_heap(HeapManager* manager, Heap* out_heap) {
	BOOL ret = FALSE;

	int minAllocIndex = 0;
	int minAllocSize = manager->alocirani_bajtovi[minAllocIndex];

	if (manager != NULL && manager->broj_heapova > 0) {
		Heap temp = manager->niz_heapova[0];
		EnterCriticalSection(&manager->manager_mutex);
			// Prolaz kroz sve Heap-ove, traži se onaj sa najmanje alociranih bajta
			for (int i = 0; i < manager->broj_heapova; i++) {
				if (manager->alocirani_bajtovi[i] != -1 && manager->alocirani_bajtovi[i] <= minAllocSize) {
					minAllocIndex = i;
					minAllocSize = manager->alocirani_bajtovi[i];
					temp = manager->niz_heapova[i];
					manager->trenutni_heap = i;
				}
			}
		LeaveCriticalSection(&manager->manager_mutex);
		
		if (temp != NULL) {
			*out_heap = temp;
			ret = TRUE;
		}
	}
	return ret;
}

// Nakon sto dobije Heap (od operacije iznad), alocira se memorija iz Heap-a
void* HeapOperacije_dobavi_memoriju(HeapManager* manager, int kolicina_memorije, Heap* out_heap) {
	if (manager != NULL && kolicina_memorije > 0) {
		Heap heap;

		if (HeapOperacije_dobavi_heap(manager, &heap)) {
			void* ret = HeapFunkcije_alociraj(kolicina_memorije, heap);
			if (ret != NULL) {
				*out_heap = heap;
				manager->alocirani_bajtovi[manager->trenutni_heap] += kolicina_memorije;
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
BOOL HeapManipulationOperations_free_memory(HeapManager* manager, void* pokazivac, Heap heap) {
	if (manager != NULL && pokazivac != NULL && heap != NULL) {
		return HeapFunkcije_dealociraj(pokazivac, heap);
	}
	else {
		return FALSE;
	}
}
