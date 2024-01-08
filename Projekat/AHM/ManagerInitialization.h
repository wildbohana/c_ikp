#pragma once
#include "..\HeapManager\InitHeapManager.h"
#include "..\HeapManager\DodajHeap.h"
#include "Structs.h"

/**
* Define koji deklarise objekte heap manager-a i recnika
* Radi se u header-u kako bi se umanjila mogucnost korisnika da rucno upravlja sa ovim objektima
*/
#ifndef MANAGER_DEFINE
	HeapManager* menadzer;
	Dictionary* recnik;
#else
	extern HeapManager* _manager;
	extern Dictionary* _dictionary;
#endif

// Funkcija koja inicijalizuje objekte HeapManager-a i recnika
// U slucaju neuspesne inicijalizacije, obe strukture ostaju neinicijalizovane
BOOL ManagerInitialization_inicijalizuj_manager(unsigned heap_count);

// Funkcija koja unistava HeapManager i recnik
// Sve strukture unutar manager-a i recnika su takodje unistene
BOOL ManagerInitialization_deinicijalizuj_manager();
