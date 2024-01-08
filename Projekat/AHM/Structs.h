#pragma once
#include "..\HeapManager\HeapManagerStruct.h"
#include "HashTable.h"

// Recnik je "wrapper" za HashTable
// HashTable nije thread-safe, pa dodajemo mutex da bi postao
typedef struct dict {
	HashTable* hash_tabela;			// Pokazivac na HashTable objekat
	Heap privatan_heap;				// Handle privatnog Heap-a recnika koji sadrzi hash tabelu i sve njene elemente
	CRITICAL_SECTION dict_mutex;	// Propusnica kriticne sekcije, koja omogucava thread-safe operacije nad recnikom
} Dictionary;
