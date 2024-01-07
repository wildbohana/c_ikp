#include "Mreza.h"

BOOL Inicijalizuj_Socket() {
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WSAStartup neuspesan, greska: %d\n", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

void Selektuj_Operaciju(SOCKET s, char operacija) {
	if (operacija == 'r') {
		// Citanje
		do {
			FD_SET set;
			TIMEVAL time_val;
			FD_ZERO(&set);
			FD_SET(s, &set);
			time_val.tv_sec = 1;
			time_val.tv_usec = 0;

			int rezultat = select(0 /* ignored */, &set, NULL, NULL, &time_val);
			if (rezultat > 0) {
				break;
			}
		} 
		while (1);
	}
	else if (operacija == 'w') {
		// Pisanje
		do {
			FD_SET set;
			TIMEVAL time_val;
			FD_ZERO(&set);
			FD_SET(s, &set);
			time_val.tv_sec = 1;
			time_val.tv_usec = 0;

			int rezultat = select(0 /* ignored */, NULL, &set, NULL, &time_val);
			if (rezultat > 0) {
				break;
			}
		} 
		while (1);
	}
}

void Posalji_Poruku(SOCKET s, char* niz, int broj_bajtova) {
	int rezultat = 0;
	int poslati_bajtovi = 0;

	Selektuj_Operaciju(s, 'w');
	rezultat = send(s, (char*)&broj_bajtova, 4, NULL);

	do {
		Selektuj_Operaciju(s, 'w');
		rezultat = send(s, niz + poslati_bajtovi, broj_bajtova - poslati_bajtovi, NULL);

		if (rezultat == 0) {
			continue;
		}
		else if (rezultat == -1) {
			printf("\nGreska!\n");
			return;
		}

		poslati_bajtovi += rezultat;
	} 
	while (poslati_bajtovi < broj_bajtova);
}

int Primi_Poruku(SOCKET s, char** niz) {
	int rezultat = 0;
	int primljeni_bajtovi = 0;

	char* bafer = (char*)malloc(4 * sizeof(char));

	// U niz smestamo 4-byte broj koji nam govori kolika je kolicina podataka koju treba da primimo kroz mrezu
	Selektuj_Operaciju(s, 'r');
	rezultat = recv(s, bafer, 4, NULL);

	// Pocetnu adresu prihvatnog bafera kastujemo na int* (4 bajta) i dereferenciramo kako bi dobili kolicinu podatka
	int ocekivani_bajtovi = *((int*)bafer);
	free(bafer);
	*niz = (char*)malloc(ocekivani_bajtovi);

	do {
		Selektuj_Operaciju(s, 'r');
		rezultat = recv(s, *niz + primljeni_bajtovi, ocekivani_bajtovi - primljeni_bajtovi, NULL);

		if (rezultat == 0) {
			continue;
		}
		else if (rezultat == -1) {
			printf("\nGreska!\n");
			break;
		}

		primljeni_bajtovi += rezultat;

	} while (primljeni_bajtovi < ocekivani_bajtovi);

	return ocekivani_bajtovi;
}
