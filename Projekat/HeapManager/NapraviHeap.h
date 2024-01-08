#pragma once
#include "Windows.h"
#include "heapapi.h"

typedef HANDLE Heap;

// Funkcija koja pravi Heap sa maksimalnom velicinom koja se prosledjuje
// Stvara Heap koji je "thread-safe" u slucaju koriscenja thread-safe operacija
// Inicijalna komitovana kolicina memorije je 1/4 maksimalne velicine Heap-a
// U slucaju da Heap nije moguce stvoriti, ili je prosledjena vrednost <=0, povratna vrednost je NULL
Heap NapraviHeap_ogranicen(int heap_size);

// Funkcija koja pravi Heap cija se velicina "beskonacno" povecava
// Stvara Heap koji je "thread-safe" u slucaju koriscenja thread-safe operacija
// Ako je prosledjena vrednost <0, stvara se Heap sa inicijalnom komitovanom velicinom od 50 MB
// U slucaju da Heap nije moguce stvoriti, povratna vrednost je NULL
Heap NapraviHeap_neogranicen(int initial_commited_bytes);

// Funkcija koja pravi Heap cija se velicina "beskonacno" povecava
// Heap nije "thread-safe" (zato je "unsafe")
// Ako je prosledjena vrednost <=0, stvara se Heap sa inicijalnom komitovanom velicinom od 50 MB
// U slucaju da Heap nije moguce stvoriti, povratna vrednost je NULL
Heap NapraviHeap_neogranicen_unsafe(int initial_commited_bytes);
