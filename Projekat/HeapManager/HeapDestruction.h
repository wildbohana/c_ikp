#pragma once
#include "Windows.h"
#include "heapapi.h"

typedef HANDLE Heap;

/**
* Funkcija koja unistava prosledjen Heap i invalidira handle na dati Heap
* Ako je prosledjena vrednost NULL, rezultat operacije ce biti FALSE
* Funkcija je "thread-safe", ako je i Heap "thread-safe"
*
* heap: Handle na heap koji je potrebno unistiti
*
* Vraca: BOOL vrednost koja oznacava da li je operacija uspesno izvrsena
*/
BOOL HeapDestruction_destroy_heap(Heap heap);
