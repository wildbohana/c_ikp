#include "DodajHeap.h"

// Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
// Njegova inicijalna komitovana memorija je jednaka 1/4 maksimalne memorije Heap-a
BOOL DodajHeap_ogranicen(HeapManager* manager, Heap* out_heap) {
	BOOL ret = FALSE;

	// Provera da li je broj Heap-ova manji od maksimalnog broja Heap-ova
	if (manager != NULL && manager->broj_heapova < manager->max_heapova) {
		// Predodredjena maksimalna velicina se uzima iz HeapManager-a
		Heap h = NapraviHeap_ogranicen(manager->velicina_heapa);
		if (h != NULL) {
			EnterCriticalSection(&manager->manager_mutex);
				// Smesti novi Heap iza poslednjeg dodatog Heap-a (ako to mesto pokazuje na NULL)
				if (manager->niz_heapova[manager->broj_heapova] == NULL) {
					manager->niz_heapova[manager->broj_heapova] = h;
					manager->alocirani_bajtovi[manager->broj_heapova] = 0;
					manager->broj_heapova += 1;
					*out_heap = h;
					ret = TRUE;
				}
			LeaveCriticalSection(&manager->manager_mutex);
		}
	}
	return ret;
}

// Funkcija koja u HeapManager dodaje Heap "beskonacne" maksimalne velicine
BOOL DodajHeap_neogranicen(HeapManager* manager, Heap* out_heap, int initial_commited_size) {
	BOOL ret = FALSE;

	// Provera da li u HeapManager-u ima mesta za novi Heap
	if (manager != NULL && manager->broj_heapova < manager->max_heapova) {
		// Stvaranje "beskonacnog" Heap-a
		Heap h = NapraviHeap_neogranicen(initial_commited_size);
		if (h != NULL) {
			EnterCriticalSection(&manager->manager_mutex);
				// Smesti Heap iza posledjeg dodatog Heap-a, ako to mesto pokazuje na NULL
				if (manager->niz_heapova[manager->broj_heapova] == NULL) {
					manager->niz_heapova[manager->broj_heapova] = h;
					manager->alocirani_bajtovi[manager->broj_heapova] = 0;
					manager->broj_heapova += 1;
					*out_heap = h;
					ret = TRUE;
				}
			LeaveCriticalSection(&manager->manager_mutex);
		}
	}
	return ret;
}
