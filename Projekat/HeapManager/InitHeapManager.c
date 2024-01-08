#include "InitHeapManager.h"

// Inicijalizacija Heap Manager-a
// Parametri su maksimalna velicina Heap-a i maksimalan broj Heap-ova koje Manager moze da ima
HeapManager* InitHeapManager_inicijalizuj_manager(int velicina_heapova, int maksimalno_heapova) {
	HeapManager* manager = (HeapManager*)malloc(sizeof(HeapManager));
	if (manager != NULL) {
		// Provera da li je maksimalan broj Heap-ova veci od 0
		if (maksimalno_heapova > 0) {
			// Lista Heap-ova
			manager->niz_heapova = (Heap*)malloc(maksimalno_heapova * sizeof(Heap));
			if (manager->niz_heapova != NULL) {
				// Ako je uspesna alokacija memorije, inicijalizuju se sva mesta za Heap-ove na NULL
				for (int i = 0; i < maksimalno_heapova; i++)
					manager->niz_heapova[i] = NULL;
			}
			else {
				// Oslobadja se vec zauzeta memorija i inicijalizacija se proglasava neuspesnom
				free(manager); 
				return NULL;
			}

			// Lista zauzete memorije
			manager->alocirani_bajtovi = (int*)malloc(maksimalno_heapova * sizeof(int));
			if (manager->alocirani_bajtovi != NULL) {
				// Ako je uspesna alokacija memorije, inicijalizuju se sva mesta za Heap-ove na NULL
				for (int i = 0; i < maksimalno_heapova; i++)
					manager->alocirani_bajtovi[i] = -1;
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
		if (velicina_heapova > 0)
			manager->velicina_heapa = velicina_heapova;
		else
			manager->velicina_heapa = 50000000;

		manager->max_heapova = maksimalno_heapova;
		manager->broj_heapova = 0;
		manager->trenutni_heap = 0;
		InitializeCriticalSection(&manager->manager_mutex);
	}
	return manager;
}

// Brisanje i Manager-a i Heap-ova
void InitHeapManager_unisti_manager(HeapManager** manager) {
	if (manager != NULL) {
		HeapManager* temp = *manager;
		if (temp != NULL) {
			// Pored oslobadjanja niza Heap-ova, vrsi se i njihovo unistavanje
			if (temp->niz_heapova != NULL) {
				for (int i = 0; i < temp->max_heapova; i++) {
					if (temp->niz_heapova[i] != NULL) {
						UnistiHeap_brisanje(temp->niz_heapova[i]);
					}
				}
				free(temp->niz_heapova);
			}

			if (temp->alocirani_bajtovi != NULL) 
				free(temp->alocirani_bajtovi);
			

			DeleteCriticalSection(&temp->manager_mutex);
			free(temp);
			manager = NULL;
		}
	}
}
