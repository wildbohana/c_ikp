#pragma once
#include <stdint.h>
#include "Windows.h"

// Element u hash tabeli (tj. u bucket-u)
// Kljuc - pokazivac na memorijski blok
// Vrednost - broj Heap-a u kom se nalazi alocirani blok
typedef struct hash_node {
	void* kljuc;				// Kljuc prema kojem se vrsi pretraga u tabeli
	void* vrednost;				// Vrednost koja se smesta u tabelu
	struct hash_node* sledeci;	// Pokazivac na sledeci element u bucket-u
} HashNode;

// Hash tabela
// U bucket idu svi elementi ciji kljuc vraca isti hash (kolizija)
// HashTable nije thread-safe (zbog toga se koristi recnik)
typedef struct hash_table {
	HashNode** bucketi;									// Bucket-i (niz pokazivaca na elemente)
	int trenutna_velicina;								// Trenutna velicina tabele
	int minimalna_velicina;								// Minimalna velicina tabele (mora biti prost broj)
	int trenutno_elemenata;								// Broj elemenata u tabeli
	void* (*funkcija_za_alokaciju_bucketa)(int);		// Pokazivac na funkciju koja alocira memoriju za bucket-e
	void(*funkcija_za_dealokaciju_bucketa)(HashNode**);	// Pokazivac na funkciju koja oslobadja memoriju za bucket-e
	void(*funkcija_za_dealokaciju_cvorova)(HashNode*);	// Pokazivac na funkciju koja oslobadja memoriju za jedan element
	void* (*funkcija_za_alokaciju_cvorova)();			// Pokazivac na funkciju koja alocira memoriju za jedan element
} HashTable;

// Funkcija koja inicjalizuje HashTable objekat
// Korisnik moze da bira da li ce koristiti unsafe ili safe funkcije za alokaciju i delokaciju memorije
// Za minimalnu velicinu je potrebno proslediti paran broj
// Ako se to ne uradi, minimalna vrednost ce biti prvi sledeci paran broj od te vrednosti (za broj manji od nule, velicina tabele ce biti 1009)
BOOL HashTable_inicijalizacija_tabele(HashTable* table, unsigned int minimal_size, void* (*bucket_list_allocating_function)(int), void(*bucket_list_free_function)(HashNode**), void* (*node_allocate_function)(), void(*node_free_function)(HashNode*));


// Funkcija koja deinicijalizuje HashTable objekat
// Operacija nije thread-safe
// Brise sve elemente iz tabele, kao i samu tabelu, ali ne dealocira prosledjen objekat
BOOL HashTable_deinicijalizacija_tabele(HashTable* table);

// Funkcija koja smesta element u tabelu
// Operacija nije thread-safe
// Tabela samostalno alocira element koji ce biti unet
// U slucaju da tabela vec sadrzi element sa datim kljucem, funkcija vraca FALSE
BOOL HashTable_ubaci_element(HashTable* table, void* key, void* value);

// Funkcija koja brise element iz tabele
// Operacija nije thread-safe
// Tabela samostalno dealocira element koji se brise iz nje
// U slucaju da je prosledjena tabela ili kljuc NULL, funkcija vraca FALSE
BOOL HashTable_izbaci_element(HashTable* table, void* key, void** out_value);

// Funkcija koja pronalazi element sa datim kljucem u tabeli
// U slucaju da je bilo koji parametar NULL, funkcija vraca NULL pokazivac
HashNode* HashTable_dobavi_element(HashTable* table, void* key);
