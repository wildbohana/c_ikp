#include "ManagerInitialization.h"
#include "Dictionary.h"

// Funkcija inicijalizuje Manager, pa tabelu
// Ako je Manager vec inicijalizovan, vraca false
// Ako Manager nije inicijalizovan, a recnik jeste, vratice true i nastavice da koristi vec inicijalizovan recnik
BOOL ManagerInitialization_inicijalizuj_manager(unsigned heap_count) {
	BOOL ret = TRUE;

	// Provera da li je heap_count > 0, vraca FALSE ako nije, ako jeste inicijalizuje HeapManager
	if (heap_count > 0)
		menadzer = InitHeapManager_inicijalizuj_manager(0, heap_count);
	else
		return FALSE;

	// Handle za Heap
	Heap heap;

	for (unsigned i = 0; i < heap_count; i++) {
		if (!DodajHeap_neogranicen(menadzer, &heap, 50000000)) {
			ret = FALSE;
			break;
		}
	}
	
	// Ako su svi Heap-ovi uspesno dodati nastavlja dalje (stvara recnik)
	// U suprotnom brise sve, i Manager-a i Heap-ove
	if (ret == FALSE)
		InitHeapManager_unisti_manager(&menadzer);
	else {
		ret = NapraviRecnik(1000);
	}

	return ret;
}

// Unistava i recnik i Manager
// Ako je bar jedno uspesno unisteno vraca true
BOOL ManagerInitialization_deinicijalizuj_manager() {
	BOOL ret = FALSE;

	if (menadzer != NULL) {
		InitHeapManager_unisti_manager(&menadzer);
		ret = TRUE;
	}

	if (recnik != NULL) {
		ObrisiRecnik();
		ret = TRUE;
	}

	return ret;
}
