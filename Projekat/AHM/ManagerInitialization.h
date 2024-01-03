#pragma once
#include "..\HeapManager\HeapManagerInitialization.h"
#include "..\HeapManager\HeapAddingOperations.h"
#include "Structs.h"

/**
* Define koji deklarise objekte heap manager-a i recnika
* Radi se u header-u kako bi se umanjila mogucnost korisnika da rucno upravlja sa ovim objektima
*/
#ifndef MANAGER_DEFINE
	HeapManager* _manager;
	Dictionary* _dictionary;
#else
	extern HeapManager* _manager;
	extern Dictionary* _dictionary;
#endif

/**
* Funkcija koja inicijalizuje objekte heap manager-a i recnika
* Vraca FALSE ako prosledjen broj heapova nije >0
* Vraca FALSE ako je neuspesna inicijalizacija manager-a ili recnika
* U slucaju neuspesne inicijalizacije, obe strukture ostaju neinicijalizovane
*
* Vraca: BOOL vrednost koja je indikator uspesnosti operacije
*/
BOOL ManagerInitialization_initialize_manager(unsigned heap_count);

/**
* Funkcija koja unistava heap manager i deinicijalizuje recnik
* Sve strukture unutar manager-a i recnika su takodje unistene
* Vraca FALSE ako manager i recnik nisu prethodno inicijalizovani
* U slucaju rucnog menjanja manager-a i recnika moze doci do exception-a
*
* Vraca: BOOL vrednost koja je indikator uspesnosti operacije.
*/
BOOL ManagerInitialization_destroy_manager();
