#pragma once
#include "..\HeapManager\Structs.h"
#include "HashTable.h"

typedef struct dict {
	HashTable* _table;     // Pokazivac na HashTable objekat
	Heap _dict_heap;       // Handle privatnog Heap-a recnika koji sadrzi hash tabelu u sve njene elemente
	CRITICAL_SECTION _cs;  // Propusnica kriticne sekcije, koja omogucava thread-safe operacije nad recnikom
} Dictionary;
