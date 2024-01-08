#pragma once
#include "Structs.h"
#include "..\HeapManager\NapraviHeap.h"
#include "..\HeapManager\UnistiHeap.h"
#include "..\HeapManager\HeapFunkcije.h"

// Recnik sa heap-ovima
extern Dictionary* recnik;

BOOL NapraviRecnik(int minimalna_velicina);
BOOL UbaciURecnik(void* key, void* value);
BOOL IzbaciIzRecnika(void* key, void** value);
void ObrisiRecnik();
