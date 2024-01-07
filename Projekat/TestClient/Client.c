#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "conio.h"

#include "../TestCommon/Mreza.h"
#include "../TestCommon/Poruka.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 27016
#define CLIENTS_NO 100

DWORD WINAPI klijentska_nit(LPVOID lpParam);
CRITICAL_SECTION cs;

// Duzine poruka - za svakog klijenta
int duzine_poruka[CLIENTS_NO];

int main() {
	// Inicijalizacija Windows soketa
	if (Inicijalizuj_Socket() == FALSE)	{
		return 1;
	}

	InitializeCriticalSection(&cs);

	DWORD ids[CLIENTS_NO];
	HANDLE niti[CLIENTS_NO];

	unsigned seed = time(0);
	srand(seed);

	// Duzine od 100-500
	for (int index = 0; index < CLIENTS_NO; index++) {
		duzine_poruka[index] = (rand() % 400) + 100;
	}

	// Stvaranje 100 klijentskih niti
	for (int i = 0; i < CLIENTS_NO; i++) {
		niti[i] = CreateThread(NULL, 0, &klijentska_nit, (LPVOID)i, 0, &ids[i]);
	}

	getchar();

	// Gasenje 100 niti
	for (int i = 0; i < CLIENTS_NO; i++) {
		CloseHandle(niti[i]);
	}

	DeleteCriticalSection(&cs);

	WSACleanup();
	return 0;
}

// Zadatak za klijentsku nit
DWORD WINAPI klijentska_nit(LPVOID param) {
	// Kriticna sekcija
	EnterCriticalSection(&cs);

	int broj_klijenta = (int)param;
	if (Inicijalizuj_Socket() == FALSE) {
		return 1;
	}
	
	printf("\nKlijent broj: %d\n", broj_klijenta);

	int rezultat;
	char* poruka;
	int duzina_poruke = 0;	

	SOCKET connect_socket = INVALID_SOCKET;
	connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect_socket == INVALID_SOCKET) {
		printf("socket nesupesan, greska: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	struct sockaddr_in serverska_adresa;
	serverska_adresa.sin_family = AF_INET;
	serverska_adresa.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);	// IP adresa servera
	serverska_adresa.sin_port = htons(SERVER_PORT);

	if (connect(connect_socket, (SOCKADDR*)&serverska_adresa, sizeof(serverska_adresa)) == SOCKET_ERROR) {
		printf("Spajanje sa serverom neuspesno.\n");
		closesocket(connect_socket);
		WSACleanup();
	}

	unsigned long int non_blocking_mode = 1;
	rezultat = ioctlsocket(connect_socket, FIONBIO, &non_blocking_mode);

	// Slanje poruke na server
	duzina_poruke = Generisi_Poruku(&poruka, duzine_poruka[broj_klijenta]);
	Selektuj_Operaciju(connect_socket, 'w');
	Posalji_Poruku(connect_socket, poruka, duzina_poruke);
	//printf("Saljem na server: %s\n", poruka);

	// Pocetak merenja vremena
	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	// Redovan malloc i free
	char* poruka_za_malloc = malloc(duzina_poruke);
	free(poruka_za_malloc);

	// Racunanje delte
	end_time = clock();
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Vreme potrebno za zauzimanje %d bajtova standardnim malloc i free je: %f\n", duzina_poruke, cpu_time_used);

	free(poruka);

	// Primanje poruke sa servera
	Selektuj_Operaciju(connect_socket, 'r');
	duzina_poruke = Primi_Poruku(connect_socket, &poruka);
	//printf("Primio od servera: %s\n", message);

	free(poruka);

	rezultat = shutdown(connect_socket, SD_BOTH);
	if (rezultat == SOCKET_ERROR) {
		printf("Shutdown neuspesan, greska: %d\n", WSAGetLastError());
		closesocket(connect_socket);
		WSACleanup();
		return 1;
	}

	// Zatvaranje soketa
	closesocket(connect_socket);
	LeaveCriticalSection(&cs);
	return 0;
}
