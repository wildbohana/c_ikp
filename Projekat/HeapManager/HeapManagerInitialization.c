#include "HeapManagerInitialization.h"

// Inicijalizacija Heap Manager-a
// Parametri su maksimalna velicina Heap-a, i maksimalan broj Heap-ova koje Manager moze da ima
HeapManager* HeapManagerInitialization_initialize_heap_manager(int heap_size, int maximal_heap_count) {
	HeapManager* manager = (HeapManager*)malloc(sizeof(HeapManager));
	if (manager != NULL) {
		// Provera da li je max_heap_count veci od 0
		if (maximal_heap_count > 0) {
			// Lista Heap-ova
			manager->heap_array = (Heap*)malloc(maximal_heap_count * sizeof(Heap));
			if (manager->heap_array != NULL) {
				// Ako je uspesna alokacija memorije, inicijalizuju se sva mesta za Heap-ove na NULL
				for (int i = 0; i < maximal_heap_count; i++)
					manager->heap_array[i] = NULL;
			}
			else {
				// Oslobadja se vec zauzeta memorija i inicijalizacija se proglasava neuspesnom
				free(manager); 
				return NULL;
			}

			// Lista zauzete memorije
			manager->heap_allocated_bytes = (int*)malloc(maximal_heap_count * sizeof(int));
			if (manager->heap_allocated_bytes != NULL) {
				// Ako je uspesna alokacija memorije, inicijalizuju se sva mesta za Heap-ove na NULL
				for (int i = 0; i < maximal_heap_count; i++)
					manager->heap_allocated_bytes[i] = -1;
			}
			else {
				// Oslobadja se vec zauzeta memorija i inicijalizacija se proglasava neuspesnom
				free(manager);
				return NULL;
			}
		}
		// Ako nije, oslobadja se vec zauzeta memorija i inicijalizacija se proglasava neuspesnom
		else {
			free(manager); 
			return NULL;
		}

		// Ako je prosledjena podrazumevana velicina za Heap, ta velicina ce se uzeti u obzir
		// Ako je prosledjena 0, onda ce podrazumevana velicina biti 50MB
		if (heap_size > 0)
			manager->heap_size = heap_size;
		else
			manager->heap_size = 50000000;

		manager->max_heaps = maximal_heap_count;
		manager->heap_count = 0;
		manager->current_heap = 0;
		InitializeCriticalSection(&manager->manager_mutex);
	}
	return manager;
}

// Brisanje i Manager-a i Heap-ova
void HeapManagerInitialization_destroy_manager(HeapManager** manager) {
	if (manager != NULL) {
		HeapManager* temp = *manager;
		if (temp != NULL) {
			// Pored oslobadjanja niza Heap-ova, vrsi se i njihovo unistavanje
			if (temp->heap_array != NULL) {
				for (int i = 0; i < temp->max_heaps; i++) {
					if (temp->heap_array[i] != NULL) {
						HeapDestruction_destroy_heap(temp->heap_array[i]);
					}
				}
				free(temp->heap_array);
			}

			if (temp->heap_allocated_bytes != NULL) 
				free(temp->heap_allocated_bytes);
			

			DeleteCriticalSection(&temp->manager_mutex);
			free(temp);
			manager = NULL;
		}
	}
}
