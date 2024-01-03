#pragma once
#include <stdint.h>
#include "Windows.h"

// Element u hash tabeli (tj. u bucket-u)
typedef struct hash_node {
	void* key;				// Kljuc prema kojem se vrsi pretraga u tabeli
	void* value;			// Vrednost koja se smesta u tabelu
	struct hash_node* next; // Pokazivac na sledeci element u bucket-u
} HashNode;

// Hash tabela
// U bucket idu svi elementi ciji kljuc vraca isti hash (kolizija)
typedef struct hash_table {
	HashNode** _table;								// Bucket-i (niz pokazivaca na elemente)
	int size;										// Trenutna velicina tabele
	int minimal_size;								// Minimalna velicina tabele (mora biti prost broj)
	int entries;									// Broj elemenata u tabeli
	void* (*bucket_list_allocating_function)(int);	// Pokazivac na funkciju koja alocira memoriju za bucket-e
	void(*bucket_list_free_function)(HashNode**);	// Pokazivac na funkciju koja oslobadja memoriju za bucket-e
	void(*node_free_function)(HashNode*);			// Pokazivac na funkciju koja oslobadja memoriju za jedan element
	void* (*node_allocate_function)();				// Pokazivac na funkciju koja alocira memoriju za jedan element
} HashTable;

/**
* Funkcija koja inicijalizuje HashTable objekat
* Prima funkcije za alokaciju tabele i njenih elemenata od strane korisnika, tako da nudi korisniku odabir politike alociranja
* U slucaju nemogucnosti da se alocira tabela, ili je neki od parametara NULL, operacija vraca FALSE
* Potrebnoje za minimalnu velicinu poslati paran broj, ako se to ne uradi, minimalna vrednost ce biti prvi sledeci paran broj od te vrednosti
* Ako se prosledi za minimalnu velicinu broj <= 0, tabela ce biti minimalne velicine 1009
*
* Vraca: BOOL vrednost koja indikuje uspesnost operacije
*/
BOOL HashTable_initialize_table(HashTable* table, unsigned int minimal_size, void* (*bucket_list_allocating_function)(int), void(*bucket_list_free_function)(HashNode**), void* (*node_allocate_function)(), void(*node_free_function)(HashNode*));

/**
* Funkcija koja deinicijalizuje HashTable objekat
* Operacija nije thread-safe
* Brise sve elemente iz tabele, kao i samu tabelu, ali ne dealocira prosledjen objekat
* U slucaju da se prosledi NULL pokazivac kao tabela, funkcija vraca FALSE
*
* Vraca: BOOL vrednost koja indikuje uspesnost operacije
*/
BOOL HashTable_deinitialize_table(HashTable* table);

/**
* Funkcija koja smesta element u tabelu
* Operacija nije thread-safe
* Tabela samostalno alocira element koji ce biti unet
* U slucaju da bilo koja prosledjena vrednost je NULL, funkcija vraca FALSE
* U slucaju da tabela vec sadrzi element sa datim kljucem, funkcija vraca FALSE
*
* Vraca: BOOL vrednost koja je indikator uspesnosti operacije
*/
BOOL HashTable_insert(HashTable* table, void* key, void* value);

/**
* Funkcija koja brise element iz tabele
* Operacija nije thread-safe
* Tabela samostalno dealocira element koji se brise iz nje
* U slucaju da je prosledjena tabela ili kljuc NULL, funkcija vraca FALSE
* U slucaju da tabela nije inicijalizovana, funkcija vraca FALSE
*
* out_value: pokazivac na pokazivac koji ce posle operacije brisanja pokazivati na vrednost koja je bila u tabeli
*
* Vraca: BOOL vrednost koja je indikator uspesnosti operacije
*/
BOOL HashTable_delete(HashTable* table, void* key, void** out_value);

/**
* Funkcija koja pronalazi element sa datim kljucem u tabeli
* U slucaju da je bilo koji parametar NULL, funkcija vraca NULL pokazivac
* U slucaju da tabela nije inicijalizovana funkcija vraca NULL
*
* Vraca: pokazivac na element cija se vrednost kljuca poklapa sa prosledjenom vrednosti kljuca
*/
HashNode* HashTable_get(HashTable* table, void* key);
