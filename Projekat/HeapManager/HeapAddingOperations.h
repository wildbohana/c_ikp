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
* Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
* Njegova inicijalna komitovana memorija je jednaka 1/4 maksimalne memorije Heap-a
* Takodje, vrsi se alokacija memorije iz novo-dodatog Heap-a
*
* Vrednost maksimalne velicine Heap-a se uzima iz prosledjenog HeapManager objekta
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* Operacija izvrsava alokaciju memorije u novo-dodat heap, ciji je broj bajta prosledjen
* U slucaju da je pokazivac na manager objekat NULL, nema mesta za novi heap u HeapManager-u ili prosledjena velicina memorije za alokaciju je <=0, rezultat operacije ce biti FALSE
* U slucaju da nije moguce stvoriti nov Heap, rezultat operacije ce biti FALSE
* U slucaju da je Heap uspesno stvoren, a nije moguce alocirati memoriju, Heap ce biti unisten i rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi heap
* out_heap: pokazivac na Handle heap-a, koji ce posle operacije pokazivati na novo-dodat heap
* memory_size: broj bajta koji je potrebno alocirati u novo-dodat-om heap-u
* out_pointer: pokazivac na pokazivac koji ce pokazivati na alociranu memoriju
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije
*/
BOOL HeapAddingOperations_add_default_heap_with_alloc(HeapManager* manager, Heap* out_heap, int memory_size, void** out_pointer);

/**
* Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
* Njegova inicijalna komitovana memorija je jednaka maksimalnoj memoriji Heap-a
* Vrednost maksimalne velicine Heap-a se prosledjuje
* 
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* U slucaju da je pokazivac na manager objekat NULL ili nema mesta za novi heap u HeapManager-u, rezultat operacije ce biti FALSE
* U slucaju da nije moguce stvoriti nov heap, rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi Heap
* out_heap: pokazivac na Handle Heap-a, koji ce posle operacije pokazivati na novo-dodat Heap
* heap_size: maksimalna velicina Heap-a u bajtima
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije
*/
BOOL HeapAddingOperations_add_big_heap(HeapManager* manager, Heap* out_heap, int heap_size);

/**
* Funkcija koja u HeapManager dodaje Heap predodredjene maksimalne velicine
* Njegova inicijalna komitovana memorija je jednaka maksimalnoj memoriji Heap-a
* Takodje, vrsi se alokacija memorije iz novo-dodatog Heap-a
* Vrednost maksimalne velicine Heap-a se prosledjuje
* 
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* Operacija izvrsava alokaciju memorije u novo-dodat heap, ciji je broj bajta jednak maksimalnoj velicini Heap-a
* U slucaju da je pokazivac na manager objekat NULL ili nema mesta za novi heap u HeapManager-u, rezultat operacije ce biti FALSE
* U slucaju da nije moguce stvoriti nov heap, rezultat operacije ce biti FALSE
* U slucaju da je heap uspesno stvoren, a nije moguce alocirati memoriju, heap ce biti unisten i rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi Heap
* out_heap: pokazivac na Handle Heap-a, koji ce posle operacije pokazivati na novo-dodat Heap
* memory_size: broj bajta koji je potrebno alocirati u novo-dodat-om Heap-u
* out_pointer: pokazivac na pokazivac koji ce pokazivati na alociranu memoriju
* out_pointer: pokazivac na pokazivac koji ce pokazivati na alociranu memoriju
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije
*/
BOOL HeapAddingOperations_add_big_heap_with_alloc(HeapManager* manager, Heap* out_heap, int heap_size, void** out_pointer);

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

/**
* Funkcija koja u heap manager dodaje heap "beskonacne" maksimalne velicine
* Njegova inicijalna komitovana memorija je jednaka prosledjenoj vrednosti
* Takodje, vrsi se alokacija memorije iz novo-dodatog Heap-a
* 
* HeapManager objekat mora biti inicijalizovan, u suprotnom ce sama operacija baciti exception
* Operacija izvrsava alokaciju memorije u novo-dodat Heap, ciji je broj bajta prosledjen
* U slucaju da je pokazivac na Manager objekat NULL, nema mesta za novi Heap u HeapManager-u ili prosledjena velicina memorije za alokaciju je <=0, rezultat operacije ce biti FALSE
* U slicaju da je prosledjena inicijalna velicina komitovane memorije <0, bice stvoren Heap sa inicijalnom komitovanom memorijom od 50 MB
* U slucaju da nije moguce stvoriti nov Heap, rezultat operacije ce biti FALSE.
* U slucaju da je Heap uspesno stvoren, a nije moguce alocirati memoriju, Heap ce biti unisten i rezultat operacije ce biti FALSE
* Operacija je "thread-safe"
*
* manager: pokazivac na inicijalizovan HeapManager objekat, u koji se smesta novi Heap
* out_heap: pokazivac na Handle heap-a, koji ce posle operacije pokazivati na novo-dodat Heap
* memory_size: broj bajta koji je potrebno alocirati u novo-dodatom Heap-u
* initial_commited_bytes: inicijana velicina komitovane memorije u byte-ima
* out_pointer: pokazivac na pokazivac koji ce pokazivati na alociranu memoriju
*
* Vraca: BOOL vrednost koja oznacava uspesnost operacije.
*/
BOOL HeapAddingOperations_add_infinite_heap_with_alloc(HeapManager* manager, Heap* out_heap, int memory_size, int initial_commited_size, void** out_pointer);
