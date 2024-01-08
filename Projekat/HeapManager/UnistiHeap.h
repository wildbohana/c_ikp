#pragma once
#include "Windows.h"
#include "heapapi.h"

typedef HANDLE Heap;

// Funkcija koja unistava prosledjen Heap i njegov Handle
// Ako je prosledjena vrednost NULL, rezultat operacije ce biti FALSE
// Funkcija je "thread-safe", ako je i Heap "thread-safe"
BOOL UnistiHeap_brisanje(Heap heap);
