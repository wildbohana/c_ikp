#pragma once
#include "..\HeapManager\HeapFunkcije.h"
#include "..\HeapManager\HeapOperacije.h"
#include "Structs.h"

// Exit kodovi prilikom greske
#ifndef ERRORS
#define MANAGER_UNINITIALIZED_ERROR -1	// Manager nije inicijalizovan
#define NULL_SENT_ERROR -2				// NULL poslat pri pozivu thread_free funkcije
#endif

// Deklaracija objekata HeapManager-a i recnika
#ifndef MANAGER_DEFINE
	HeapManager* menadzer;
	Dictionary* recnik;
#else
	extern HeapManager* _manager;
	extern Dictionary* _dictionary;
#endif

// Funkcija koja alocira memoriju iz manager-a
// Omogucava bolje performanse tako sto alocira memoriju iz heap pool-a
// Ako je prosledjen broj byte-a manji od nule ili sistem nije u mogucnosti da pruzi trazenu memoriju, operacija vraca NULL pokazivac
// U slucaju da recnik ili manager nisu inicijalizovani, program prekida sa radom uz izlazni kod -2
void* ahm_malloc(int bytes);

// Funkcija koja oslobadja prethodno alociran memorijski blok
// Potrebno je proslediti pokazivac koji nije NULL, i koji pokazuje na pocetak memorijskog bloka
// U slucaju da recnik ili manager nisu inicijalizovani, program baca exception sa izlaznim kodom -1
// U slucaju da je prosledjen NULL pokazivac, program prekida sa radom uz izlazni kod -2
void ahm_free(void* pokazivac);
