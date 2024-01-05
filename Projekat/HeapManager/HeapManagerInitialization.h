#pragma once
#include "Structs.h"
#include "HeapCreation.h"
#include "HeapDestruction.h"

/**
* Kreira i inicijalizuje Heap Manager
* Heap Manager ce se nalaziti na procesnom default Heap-u
* U slucaju neuspeha operacije se vraca NULL
* Ako je prosledjen maksimalan broj Heap-ova <=0, rezultat operacije ce biti NULL
* U slucaju da je prosledjena velicina Heap-a <=0, velicina "default" Heap-a ce biti 50 MB
*
* heap_size: velicina heap-a u byte-ima, koja predstavlja maksimalnu velicinu "default" heap-a
* heap_count: maksimalan broj heap-ova
*
* Vraca: pokazivac na inicijalizovan HeapManager
*/
HeapManager* HeapManagerInitialization_initialize_heap_manager(int heap_size, int maximal_heap_count);

/**
* Funkcija unistava Heap Manager i postavlja prosledjeni pokazivac na Manager na NULL
* Funkcija ce unistiti i Heap-ove i invalidirati njihove Handle-ove
* Funkcija ce osloboditi memoriju Manager objekta
*
* manager: pokazivac na pokazivac na objekat Heap Managera kojeg je potrebno unistiti
*/
void HeapManagerInitialization_destroy_manager(HeapManager** manager);
