#pragma once
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Inicijalizuje windows socket za tekuci proces, vraca TRUE ako se izvrsi uspesno, u suprotnom FALSE
BOOL Inicijalizuj_Socket();

// Osluskuje da li je moguce slanje ili primanje poruka
void Selektuj_Operaciju(SOCKET s, char operation);

// Salje poruku nasumicne duzine
void Posalji_Poruku(SOCKET s, char* niz, int brojBajtova);

// Prima poruku nasumicne duzine
// Vraca broj primljenih bajta
int Primi_Poruku(SOCKET s, char** niz);
