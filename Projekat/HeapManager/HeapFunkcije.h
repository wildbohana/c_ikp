#pragma once
#include "Windows.h"
#include "heapapi.h"

typedef HANDLE Heap;

// Alokacija memorije iz Heap-a
// Funkcija je "thread-safe", ako je i heap "thread-safe"
// U slucaju da je prosledjena vrednost byte-a manja od nule, ili je Heap NULL, funkcija vraca NULL pokazivac
// U slucaju da sistem nije u mogucnosti da suplementira potrebnu memoriju, funkcija vraca NULL pokazivac
void* HeapFunkcije_alociraj(int bajta, Heap heap);

// Alokacije memorije iz Heap-a
// Funkcija nije "thread-safe" (stoga "unsafe")
// U slucaju da je prosledjena vrednost byte-a manja od nule, ili je Heap NULL, funkcija vraca NULL pokazivac
// U slucaju da sistem nije u mogucnosti da suplementira potrebnu memoriju, funkcija vraca NULL pokazivac
void* HeapFunkcije_alociraj_unsafe(int bajta, Heap heap);

// Dealokacije memorije iz Heap-a
// Funkcija je "thread-safe", ako je i Heap "thread-safe"
// U slucaju da je prosledjen pointer NULL pokazivac, ili je Heap NULL, funkcija vraca FALSE
// U slucaju da pokazivac pokazuje na Heap koji je drugaciji od prosledjenog, aplikacija baca exception
BOOL HeapFunkcije_dealociraj(void* pokazivac, Heap heap);

// Dealokacija memorije iz Heap-a
// Funkcija nije "thread-safe" (stoga "unsafe")
// U slucaju da je prosledjen pointer NULL pokazivac, ili je Heap NULL, funkcija vraca FALSE
// U slucaju da pokazivac pokazuje na Heap koji je drugaciji od prosledjenog, aplikacija baca exception
BOOL HeapFunkcije_dealociraj_unsafe(void* pokazivac, Heap heap);
