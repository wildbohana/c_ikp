#pragma once
#include "HeapManagerStruct.h"
#include "NapraviHeap.h"
#include "HeapFunkcije.h"
#include "UnistiHeap.h"

// U HeapManager dodaje Heap "beskonacne" maksimalne velicine
// Pri inicijalizaciji se zauzima 1/4 maksimalne velicine Heap-a
// Vrednost maksimalne velicine Heap-a se uzima iz prosledjenog HeapManager objekta
// Ne moze da raste
BOOL DodajHeap_ogranicen(HeapManager* manager, Heap* out_heap);

// U HeapManager dodaje Heap "beskonacne" maksimalne velicine
// Pri inicijalizaciji se zauzima prosledjena kolicina bajtova
// U slicaju da je prosledjena inicijalna velicina komitovane memorije manji od nule, bice stvoren Heap sa inicijalnom komitovanom velicinom od 50 MB
// Moze da menja velicinu po potrebi (da raste)
BOOL DodajHeap_neogranicen(HeapManager* manager, Heap* out_heap, int initial_commited_size);
