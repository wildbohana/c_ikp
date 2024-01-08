#pragma once
#include "HeapManagerStruct.h"
#include "HeapFunkcije.h"

// Dobavlja Handle za Heap u HeapManager-u
// Odredjivanje povratnog Heap-a se vrsi pronalazenjem Heap-a sa najmanje alocirane memorije
// U slucaju da je prosledjeni Manager objekat NULL, rezultat operacije ce biti FALSE
// U slucaju da se u Manager-u ne nalazi nijedan Heap, rezultat operacije ce biti FALSE
BOOL HeapOperacije_dobavi_heap(HeapManager* manager, Heap* out_heap);

// Dobavlja memoriju iz trenutnog Heap-a
// Odredjivanje povratnog Heap-a se vrsi pronalazenjem Heap-a sa najmanje alocirane memorije
// U slucaju da je prosledjeni Manager objekat NULL ili je prosledjena velicina memorije <=0, rezultat operacije ce biti FALSE
// U slucaju da se u Manager-u ne nalazi nijedan Heap, rezultat operacije ce biti FALSE
// Operacija je "thread-safe"
void* HeapOperacije_dobavi_memoriju(HeapManager* manager, int kolicina_memorije, Heap* out_heap);


// Oslobadja prethodno zauzetu memoriju iz prosledjenog Heap-a
// U slucaju da je prosledjeni Manager objekat NULL, pokazivac NULL ili Heap NULL, rezultat operacije ce biti FALSE
// U slucaju da prosledjeni pokazivac ne pokazuje na pocetak bloka koji je uzet iz prosledjenog Heap-a, operacija ce baciti exception
// Operacija je "thread-safe" ako je i sam Heap thread-safe
BOOL HeapManipulationOperations_free_memory(HeapManager* manager, void* pokazivac, Heap heap);
