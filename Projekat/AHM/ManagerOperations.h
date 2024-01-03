#pragma once
#include "..\HeapManager\HeapManipulationOperations.h"
#include "..\HeapManager\HeapManipulation.h"
#include "Structs.h"

/**
* Define koji definise exit code-ove prilikom greske
*/
#ifndef ERRORS
#define MANAGER_UNINITIALIZED_ERROR -1	// Manager nije inicijalizovan
#define NULL_SENT_ERROR -2				// NULL poslat pri pozivu thread_free funkcije
#endif

/**
* Define koji deklarise objekte heap manager-a i recnika.
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
* Funkcija koja alocira memoriju iz manager-a
* Omogucava bolje performanse tako sto alocira memoriju iz heap pool-a
* Ako je prosledjen broj byte-a <= 0 ili sistem nije u mogucnosti da pruzi trazenu memoriju, operacija vraca NULL pokazivac
* U slucaju da recnik ili manager nisu inicijalizovani, program prekida sa radom uz izlazni kod -2
*
* bytes: broj byte-a koji se alociraju
*
* Vraca: pokazivac na novo alociranu memoriju
*/
void* advanced_malloc(int bytes);

/**
* Funkcija koja oslobadja prethodno alociran memorijski blok
* Potrebno je proslediti pokazivac koji nije NULL, i koji pokazuje na pocetak memorijskog bloka
* U slucaju da recnik ili manager nisu inicijalizovani, program baca exception sa izlaznim kodom -1
* U slucaju da je prosledjen NULL pokazivac, program prekida sa radom uz izlazni kod -2
*
* pointer: pokazivac na pocetak memorijskog bloka koji je potrebno osloboditi.
*/
void advanced_free(void* pointer);
