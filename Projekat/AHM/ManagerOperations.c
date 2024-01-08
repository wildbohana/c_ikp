#include "ManagerOperations.h"
#include "Dictionary.h"

// Zauzima trazenu memoriju (tako sto od HeapManager-a trazi memoriju)
// Ako se memorija uspesno alocira, ubacuje se u recnik pokazivac->heap, sto omogucuje dealokaciju memorije
void* ahm_malloc(int broj_bajta) {
	// Ako ne postoji Manager ili recnik, izadji iz aplikacije
	if (menadzer == NULL && recnik == NULL) {
		exit(MANAGER_UNINITIALIZED_ERROR);
	}

	// Handler za Heap
	Heap heap;

	void* pokazivac = NULL;
	if (broj_bajta > 0) {
		pokazivac = HeapOperacije_dobavi_memoriju(menadzer, broj_bajta, &heap);
		BOOL ubacen = FALSE;

		if (pokazivac != NULL) {
			ubacen = UbaciURecnik(pokazivac, heap);
		}

		if (ubacen == FALSE && pokazivac != NULL) {
			HeapFunkcije_dealociraj(pokazivac, heap);
			pokazivac = NULL;
		}
	}

	return pokazivac;
}

// Oslobadja memorijski blok na koji pokazuje pokazivac
// Trazi u recniku pokazivac, kako bi dobio Heap na koji je alocirana memorija, a potom radi i oslobadjanje memorije iz tog Heap-a
void ahm_free(void* pokazivac) {
	if (pokazivac == NULL) {
		exit(NULL_SENT_ERROR);
	}

	if (recnik == NULL && menadzer == NULL) {
		exit(MANAGER_UNINITIALIZED_ERROR);
	}

	// Handler za Heap
	Heap heap = NULL;

	BOOL izbacen = IzbaciIzRecnika(pokazivac, &heap);
	if (izbacen) {
		HeapManipulationOperations_free_memory(menadzer, pokazivac, heap);
	}
}
