#include "HeapAddingOperations.h"

// Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
// Njegova inicijalna komitovana memorija je jednaka 1/4 maksimalne memorije Heap-a
BOOL HeapAddingOperations_add_default_heap(HeapManager* manager, Heap* out_heap) {
	BOOL ret = FALSE;

	// Provera da li je broj Heap-ova manji od maksimalnog broja Heap-ova
	if (manager != NULL && manager->heap_count < manager->max_heaps) {
		// Predodredjena maksimalna velicina se uzima iz HeapManager-a
		// HeapCreation racuna 1/4 te memorije i zauzima je
		Heap h = HeapCreation_create_default_heap(manager->heap_size);
		if (h != NULL) {
			EnterCriticalSection(&manager->manager_mutex);
				// Smesti novi Heap iza poslednjeg dodatog Heap-a (ako to mesto pokazuje na NULL)
				if (manager->heap_array[manager->heap_count] == NULL) {
					manager->heap_array[manager->heap_count] = h;
					manager->heap_allocated_bytes[manager->heap_count] = 0;
					manager->heap_count += 1;
					*out_heap = h;
					ret = TRUE;
				}
			LeaveCriticalSection(&manager->manager_mutex);
		}
	}
	return ret;
}

// Funkcija koja u HeapManager dodaje Heap "beskonacne" maksimalne velicine
// Njegova inicijalna komitovana memorija jednaka je prosledjenoj vrednosti
BOOL HeapAddingOperations_add_infinite_heap(HeapManager* manager, Heap* out_heap, int initial_commited_size) {
	BOOL ret = FALSE;

	if (manager != NULL && manager->heap_count < manager->max_heaps) {
		// Stvaranje "beskonacnog" Heap-a
		Heap h = HeapCreation_create_infinite_heap(initial_commited_size);
		if (h != NULL) {
			EnterCriticalSection(&manager->manager_mutex);
				// Smesti Heap iza posledjeg dodatog Heap-a, ako to mesto pokazuje na NULL
				if (manager->heap_array[manager->heap_count] == NULL) {
					manager->heap_array[manager->heap_count] = h;
					manager->heap_allocated_bytes[manager->heap_count] = 0;
					manager->heap_count += 1;
					*out_heap = h;
					ret = TRUE;
				}
			LeaveCriticalSection(&manager->manager_mutex);
		}
	}
	return ret;
}
