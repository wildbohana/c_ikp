#include "Dictionary.h"

// Pomocna funkcija
// Oslobadja memoriju elementa iz HashTable
inline void funkcija_oslobodi_cvor(HashNode* cvor) {
	HeapFunkcije_dealociraj_unsafe(cvor, recnik->privatan_heap);
}

// Pomocna funkcija
// Zauzima memoriju za element iz HashTable
inline void* funkcija_alociraj_cvor() {
	return HeapFunkcije_alociraj_unsafe(sizeof(HashNode), recnik->privatan_heap);
}

// Pomocna funkcija
// Zauzima memoriju za bucket elemenata iz HashTable
inline void* funkcija_alociraj_buckete(int baketi) {
	return HeapFunkcije_alociraj_unsafe(sizeof(HashNode*) *baketi, recnik->privatan_heap);
}

// Pomocna funkcija
// Oslobadja memoriju za bucket elemenata iz HashTable
inline void funkcija_dealociraj_buckete(HashNode** tabela) {
	HeapFunkcije_dealociraj_unsafe(tabela, recnik->privatan_heap);
}

// Inicijalizacija recnika
BOOL NapraviRecnik(int minimalna_velicina) {
	BOOL is_ok = FALSE;

	recnik = malloc(sizeof(Dictionary));
	if (recnik != NULL) {
		// Windows.h syscall
		InitializeCriticalSection(&recnik->dict_mutex);

		// Pokusava da napravi privatni Heap (50MB) u koji ce biti smesteni Heap tabela i svi njeni elementi
		// Takodje, pokusava da napravi Hash tabelu i tada vraca TRUE
		if ((recnik->privatan_heap = NapraviHeap_neogranicen_unsafe(0)) != NULL) {
			recnik->hash_tabela = HeapFunkcije_alociraj(sizeof(HashTable), recnik->privatan_heap);

			// Inicijalizuje Hash tabelu
			if (recnik->hash_tabela != NULL && HashTable_inicijalizacija_tabele(recnik->hash_tabela, minimalna_velicina, funkcija_alociraj_buckete, funkcija_dealociraj_buckete, funkcija_alociraj_cvor, funkcija_oslobodi_cvor)) {
				is_ok = TRUE;
			}
			else {
				// Ako ne uspe, brise se memorija zauzeta za Heap
				DeleteCriticalSection(&recnik->dict_mutex);
				UnistiHeap_brisanje(recnik->privatan_heap);
			}
		}
		else {
			DeleteCriticalSection(&recnik->dict_mutex);
		}
	}
	return is_ok;
}

// Ubacivanje novog elementa u recnik (tj u HashTable)
BOOL UbaciURecnik(void* key, void* value) {
	BOOL ubacen = FALSE;
	EnterCriticalSection(&recnik->dict_mutex);
		ubacen = HashTable_ubaci_element(recnik->hash_tabela, key, value);
	LeaveCriticalSection(&recnik->dict_mutex);
	return ubacen;
}

// Brisanje elementa iz HashTable
BOOL IzbaciIzRecnika(void* key, void** value) {
	BOOL izbacen = FALSE;
	EnterCriticalSection(&recnik->dict_mutex);
		izbacen = HashTable_izbaci_element(recnik->hash_tabela, key, value);
	LeaveCriticalSection(&recnik->dict_mutex);
	return izbacen;
}

void ObrisiRecnik() {
	if (recnik != NULL) {
		// Deinicijalizacija HashTable i oslobadjanje memorije
		EnterCriticalSection(&recnik->dict_mutex);
			HashTable_deinicijalizacija_tabele(recnik->hash_tabela);
			HeapFunkcije_dealociraj(recnik->hash_tabela, recnik->privatan_heap);
		LeaveCriticalSection(&recnik->dict_mutex);

		// Unistavanje privatnog Heap-a recnika
		DeleteCriticalSection(&recnik->dict_mutex);
		UnistiHeap_brisanje(recnik->privatan_heap);
		free(recnik);
	}
}
