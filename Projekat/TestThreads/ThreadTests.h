#pragma once
#include "stdio.h"
#include "windows.h"
#include "time.h"
#include "..\AHM\ManagerInitialization.h"
#include "..\AHM\ManagerOperations.h"

/**
* Funkcija niti koja zauzima i oslobadja 3GB memorije uz pomoc advanced_heap() funkcija.
*
* Param: Kolicina memorije koju je potrebno da zauzme svaki od thread-ova.
*/
DWORD WINAPI ThreadTests_custom_malloc_and_custom_free(LPVOID Param);

/**
* Funkcija niti koja zauzima i oslobadja 3GB memorije uz pomoc malloc i free funkcija.
*
* Param: Kolicina memorije koju je potrebno da zauzme svaki od thread-ova.
*/
DWORD WINAPI ThreadTests_malloc_and_free(LPVOID Param);

/**
* Funkcija koja kreira thread-ove i meri vreme koje je potrebno za zauzimanje i oslobadjanje 3GB memorije sa advanced_heap() funkcijama.
*
* number_of_threads: Broj thread-ova koji se koristi za zauzimanje 3GB memorije.
*/
void ThreadTests_custom_malloc_initialize(int number_of_threads);

/**
* Funkcija koja kreira thread-ove i meri vreme koje je potrebno za zauzimanje i oslobadjanje 3GB memorije sa malloc() i free() funkcijama.
*
* number_of_threads: Broj thread-ova koji se koristi za zauzimanje 3GB memorije.
*/
void ThreadTests_malloc_initialize(int number_of_threads);
