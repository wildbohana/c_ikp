#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../TestCommon/Mreza.h"
#include "../TestCommon/Poruka.h"
#include "../AHM/ManagerInitialization.h"
#include "../AHM/ManagerOperations.h"

#pragma warning (disable:4996)
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "27016"
#define CLIENTS_NO 100

DWORD WINAPI worker_thread_funkcija(LPVOID lpParam);
CRITICAL_SECTION cs;

int main(void) {
	// Inicijalizacija Managera sa 5 Heap-ova
	ManagerInitialization_inicijalizuj_manager(5);

	InitializeCriticalSection(&cs);
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET accepted_socket = INVALID_SOCKET;
	int rezultat;

	if (Inicijalizuj_Socket() == FALSE) {
		return 1;
	}

	ADDRINFO* resulting_address = NULL;
	ADDRINFO hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // Socket address will be used in bind() call

	// ANSI host name -> adresa
	rezultat = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resulting_address);
	if (rezultat != 0)
	{
		printf("getaddrinfo neuspesan, greska: %d\n", rezultat);
		WSACleanup();
		return 1;
	}

	// IPv4, stream, TCP socket
	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_socket == INVALID_SOCKET)
	{
		printf("socket neuspesan, greska: %ld\n", WSAGetLastError());
		freeaddrinfo(resulting_address);
		WSACleanup();
		return 1;
	}

	// Bind soketa za adresu
	rezultat = bind(listen_socket, resulting_address->ai_addr, (int)resulting_address->ai_addrlen);
	if (rezultat == SOCKET_ERROR)
	{
		printf("bind neuspesan, greska: %d\n", WSAGetLastError());
		freeaddrinfo(resulting_address);
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	// Vise se ne koristi, obrisi
	freeaddrinfo(resulting_address);

	// Zapocni prisluskivanje na soketu
	rezultat = listen(listen_socket, SOMAXCONN);
	if (rezultat == SOCKET_ERROR)
	{
		printf("listen neuspesan, greska: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	printf("Server pokrenut, cekaju se klijenti...\n");

	// E sad ide glavni deo
	char* poruka;
	int duzina = 0;
	unsigned long rezim = 1;

	rezultat = ioctlsocket(listen_socket, FIONBIO, &rezim);
	if (rezultat != NO_ERROR)
	{
		printf("ioctlsocket za listen neuspesan, greska: %ld\n", rezultat);
	}

	rezultat = listen(listen_socket, SOMAXCONN);

	HANDLE prihvacen_thread[CLIENTS_NO];
	DWORD ids[CLIENTS_NO];
	int brojac = 0;

	while (1) {
		if (brojac != CLIENTS_NO) {
			Selektuj_Operaciju(listen_socket, 'r');
		}
		else {
			break;
		}

		struct sockaddr_in klijentska_adresa;
		int velicina_klijentske_adrese = sizeof(struct sockaddr_in);
		
		// Primi poruku sa soketa
		accepted_socket = accept(listen_socket, (struct sockaddr*)&klijentska_adresa, &velicina_klijentske_adrese);
		if (accepted_socket == INVALID_SOCKET) {
			printf("accept neuspesan, greska: %d\n", WSAGetLastError());
			closesocket(listen_socket);
			WSACleanup();
			return 1;
		}
		else {
			if (ioctlsocket(accepted_socket, FIONBIO, &rezim) != 0)	{
				printf("ioctlsocket neuspesan.");
				continue;
			}

			// Prihvati zahtev, generisi worker thread
			printf("\nNovi klijentski zahtev prihvacen (%d). Adresa klijenta: %s : %d\n", brojac, inet_ntoa(klijentska_adresa.sin_addr), ntohs(klijentska_adresa.sin_port));
			prihvacen_thread[brojac] = CreateThread(NULL, 0, &worker_thread_funkcija, accepted_socket, 0, &ids[brojac]);
			brojac++;
		}
	}

	for (int i = 0; i < CLIENTS_NO; i++) {
		WaitForSingleObject(prihvacen_thread[brojac], INFINITE);
	}

	getchar();

	rezultat = shutdown(listen_socket, SD_SEND);
	if (rezultat == SOCKET_ERROR) {
		printf("shutdown neuspesan, greska: %d\n", WSAGetLastError());
		closesocket(accepted_socket);
		WSACleanup();
		return 1;
	}

	for (int i = 0; i < CLIENTS_NO; i++) {
		CloseHandle(prihvacen_thread[i]);
	}

	DeleteCriticalSection(&cs);

	// Brisanje Manager-a, zatvaranje soketa
	ManagerInitialization_deinicijalizuj_manager();
	closesocket(listen_socket);
	closesocket(accepted_socket);
	WSACleanup();

	return 0;
}

// Funkcija za worker nit
DWORD WINAPI worker_thread_funkcija(LPVOID param) {
	SOCKET klijentski_socket = (SOCKET)param;
	char* poruka;

	// Server cita poruku od klijenata
	Selektuj_Operaciju(klijentski_socket, 'r');
	int primljenih_bajtova = Primi_Poruku(klijentski_socket, &poruka);
	//printf("Primio od klijenta: %s\n", poruka);

	// Startuj tajmer
	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	// AHM malloc i free
	char* poruka_za_ahm = ahm_malloc(primljenih_bajtova);
	ahm_free(poruka_za_ahm);

	// Izracunaj deltu
	end_time = clock();
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Vreme potrebno za zauzimanje %d bajtova advanced malloc i free je : %f\n", primljenih_bajtova, cpu_time_used);

	free(poruka);

	// Vracanje poruke nasumicne duzine
	int duzina = Generisi_Poruku(&poruka, 20);
	Selektuj_Operaciju(klijentski_socket, 'w');
	Posalji_Poruku(klijentski_socket, poruka, duzina);

	// Zatvori soket i oslobodi memoriju za poruku
	closesocket(klijentski_socket);
	free(poruka);
}
