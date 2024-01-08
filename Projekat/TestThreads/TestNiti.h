#pragma once
#include "stdio.h"
#include "windows.h"
#include "time.h"
#include "..\AHM\ManagerInitialization.h"
#include "..\AHM\ManagerOperations.h"

// Funkcija niti koja zauzima i oslobadja 4GB memorije koristeci AHM
// Param - kolicina memorije koju svaka nit treba da zauzme
DWORD WINAPI test_ahm_funkcije(LPVOID Param);

// Funkcija niti koja zauzima i oslobadja 4GB memorije koristeci ugradjene funkcije
// Param - kolicina memorije koju svaka nit treba da zauzme
DWORD WINAPI test_ugradjene_funkcije(LPVOID Param);

// Funkcija koja kreira niti koje koriste AHM funkcije
// Meri vreme potrebno za alokaciju i oslobadjanje 4GB memorije
void test_ahm_init(int broj_niti);

// Funkcija koja kreira niti koje koriste ugradjene funkcije
// Meri vreme potrebno za alokaciju i oslobadjanje 4GB memorije
void test_obican_init(int broj_niti);
