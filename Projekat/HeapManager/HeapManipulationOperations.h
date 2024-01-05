#pragma once
#include "Structs.h"
#include "HeapManipulation.h"

/**
* Dobavlja Handle na Heap u HeapManageru
* Odredjivanje povratnog Heap-a se vrsi pronalazenjem Heap-a sa najmanje alocirane memorije
* U slucaju da je prosledjeni Manager objekat NULL, rezultat operacije ce biti FALSE
* U slucaju da se u Manager-u ne nalazi nijedan Heap, rezultat operacije ce biti FALSE
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
*
* manager: pokazivac na HeapManager objekta koji sadrzi Heap koji ce biti vracen
* out_heap: pokazivac na Heap objekat, koji ce biti izmenjen tako da pokazuje na odabran Heap
*
* Vraca: BOOL vrednost koja indicira na uspesnost izvrsene operacije
*/
BOOL HeapManipulationOperations_get_heap(HeapManager* manager, int* out_heap_idx);

/**
* Dobavlja memoriju iz trenutnog Heap-a
* Odredjivanje povratnog Heap-a se vrsi pronalazenjem Heap-a sa najmanje alocirane memorije
* U slucaju da je prosledjeni Manager objekat NULL ili je prosledjena velicina memorije <=0, rezultat operacije ce biti FALSE
* U slucaju da se u Manager-u ne nalazi nijedan Heap, rezultat operacije ce biti FALSE
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* Operacija je "thread-safe"
*
* manager: pokazivac na HeapManager objekta koji sadrzi Heap koji ce biti vracen
* memory_size: velicina memorije u bajtima koju je potrebno alocirati
* out_heap: pokazivac na Heap objekat, koji ce biti izmenjen tako da pokazuje na odabran Heap
*
* Vraca: BOOL vrednost koja indicira na uspesnost izvrsene operacije
*/
void* HeapManipulationOperations_get_memory(HeapManager* manager, int memory_size, Heap* out_heap);

/**
* Oslobadja prethodno zauzetu memoriju iz prosledjenog Heap-a
* U slucaju da je prosledjeni Manager objekat NULL, pokazivac NULL ili Heap NULL, rezultat operacije ce biti FALSE
* U slucaju da prosledjeni pokazivac ne pokazuje na pocetak bloka koji je uzet iz prosledjenog Heap-a, operacija ce baciti exception
* Operacija je "thread-safe" ako je i sam Heap thread-safe
*
* manager: pokazivac na HeapManager objekta koji sadrzi Heap koji ce biti vracen
* pointer: pokazivac na zauzetu memoriju koju je potrebno osloboditi
* heap: Heap u kojeg je potrebno vratiti zauzetu memoriju
*
* return: BOOL vrednost koja indicira na uspesnost izvrsene operacije
*/
BOOL HeapManipulationOperations_free_memory(HeapManager* manager, void* pointer, Heap heap);
