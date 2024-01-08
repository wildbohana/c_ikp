#pragma once
#include "HeapManagerStruct.h"
#include "NapraviHeap.h"
#include "UnistiHeap.h"

// Kreira i inicijalizuje HeapManager
// U slucaju neuspeha operacije se vraca NULL
// Ako je prosledjen maksimalan broj Heap-ova <=0, rezultat operacije ce biti NULL
// U slucaju da je prosledjena maksimalna velicina Heap-a <=0, velicina "default" Heap-a ce biti 50 MB
HeapManager* InitHeapManager_inicijalizuj_manager(int velicina_heapova, int maksimalno_heapova);

// Funkcija unistava Heap Manager i postavlja prosledjeni pokazivac na Manager na NULL
// Funkcija ce unistiti i Heap-ove i invalidirati njihove Handle-ove
// Funkcija ce osloboditi memoriju Manager objekta
void InitHeapManager_unisti_manager(HeapManager** manager);
