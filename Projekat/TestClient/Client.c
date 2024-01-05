#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "conio.h"

#include "../TestCommon/BaseNetworkOperations.h"
#include "../TestCommon/Message.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 27016
#define CLIENTS_NO 100

DWORD WINAPI client_thread(LPVOID lpParam);
CRITICAL_SECTION cs;

int lengths[CLIENTS_NO];	//random duzine poruka

int main() {
	if (Base_initialize_windows_sockets() == FALSE)
	{
		return 1;
	}

	InitializeCriticalSection(&cs);
	DWORD ids[CLIENTS_NO];
	HANDLE threads[CLIENTS_NO];

	unsigned seed = time(0);

	srand(seed);
	int random_integer;
	for (int index = 0; index < CLIENTS_NO; index++)
	{
		lengths[index] = (rand() % 400) + 100;
	}

	for (int i = 0; i < CLIENTS_NO; i++)
	{
		threads[i] = CreateThread(NULL, 0, &client_thread, (LPVOID)i, 0, &ids[i]);
	}

	getchar();

	for (int i = 0; i < CLIENTS_NO; i++)
	{
		CloseHandle(threads[i]);
	}
	DeleteCriticalSection(&cs);

	WSACleanup();

	return 0;

}

DWORD WINAPI client_thread(LPVOID param)
{
	EnterCriticalSection(&cs);

	int n = (int)param;
	if (Base_initialize_windows_sockets() == FALSE)
	{
		return 1;
	}

	printf("\nClient NO: %d\n", n);

	SOCKET connect_socket = INVALID_SOCKET;
	int result;
	char* message;
	int length = 0;

	connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connect_socket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	struct sockaddr_in server_address;


	server_address.sin_family = AF_INET;							// IPv4 protocol
	server_address.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);	// ip address of server
	server_address.sin_port = htons(SERVER_PORT);

	if (connect(connect_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connect_socket);
		WSACleanup();
	}

	unsigned long int non_blocking_mode = 1;
	result = ioctlsocket(connect_socket, FIONBIO, &non_blocking_mode);

	length = Data_generate_message(&message, lengths[n]);
	Base_custom_select(connect_socket, 'w');
	Base_custom_send(connect_socket, message, length);
	//printf("Saljem na server: %s\n", message);


	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	char* message_for_classic_malloc = malloc(length);
	free(message_for_classic_malloc);


	end_time = clock();
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Vreme potrebno za zauzimanje %d bajtova standardnim malloc i free je : %f\n", length, cpu_time_used);

	free(message);

	Base_custom_select(connect_socket, 'r');
	length = Base_custom_recieve(connect_socket, &message);

	//printf("Primio od servera: %s\n", message);

	free(message);
	//LeaveCriticalSection(&cs);
	result = shutdown(connect_socket, SD_BOTH);

	// Check if connection is succesfully shut down.
	if (result == SOCKET_ERROR)
	{
		printf("Shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connect_socket);
		WSACleanup();
		return 1;
	}

	closesocket(connect_socket);
	LeaveCriticalSection(&cs);
	//ReleaseSemaphore(hSemaphores[(n + 1) % CLIENTS_NO-1], 1, NULL);
	return 0;
}
