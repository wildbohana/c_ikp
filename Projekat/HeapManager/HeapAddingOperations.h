#pragma once
#include "Structs.h"
#include "HeapCreation.h"
#include "HeapManipulation.h"
#include "HeapDestruction.h"

/**
* Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
* Njegova inicijalna komitovana memorija je jednaka 1/4 maksimalne memorije Heap-a
* 
* Vrednost maksimalne velicine Heap-a se uzima iz prosledjenog HeapManager objekta
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* U slucaju da je pokazivac na Manager objekat NULL ili nema mesta za novi Heap u HeapManager-u, rezultat operacije ce biti FALSE
* U slucaju da nije moguce stvoriti nov Heap, rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi heap
* out_heap: pokazivac na Handle heap-a, koji ce posle operacije pokazivati na novo-dodat heap
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije
*/
BOOL HeapAddingOperations_add_default_heap(HeapManager* manager, Heap* out_heap);

/**
* Funkcija koja u HeapManager dodaje Heap "beskonacne" maksimalne velicine, cija je inicijalna komitovana memorija jednaka prosledjenoj vrednosti
* 
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* U slucaju da je pokazivac na Manager objekat NULL ili nema mesta za novi heap u HeapManager-u, rezultat operacije ce biti FALSE
* U slicaju da je prosledjena inicijalna velicina komitovane memorije <0, bice stvoren heap sa inicijalnom komitovanom memorijom od 50 MB
* U slucaju da nije moguce stvoriti nov Heap, rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi Heap
* out_heap: pokazivac na Handle Heap-a, koji ce posle operacije pokazivati na novo-dodat Heap
* initial_commited_bytes: inicijana velicina komitovane memorije u byte-ima
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije
*/
BOOL HeapAddingOperations_add_infinite_heap(HeapManager* manager, Heap* out_heap, int initial_commited_size);
