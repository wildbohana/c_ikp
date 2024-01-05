#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../TestCommon/BaseNetworkOperations.h"
#include "../TestCommon/Message.h"
#include "../AHM/ManagerInitialization.h"
#include "../AHM/ManagerOperations.h"

#pragma warning (disable:4996)
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "27016"
#define CLIENTS_NO 100

DWORD WINAPI worker_thread_function(LPVOID lpParam);
CRITICAL_SECTION cs;
int  main(void)
{
	ManagerInitialization_initialize_manager(5);
	InitializeCriticalSection(&cs);
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET accepted_socket = INVALID_SOCKET;
	int result;

	if (Base_initialize_windows_sockets() == FALSE)
	{
		return 1;
	}

	ADDRINFO* resulting_address = NULL;
	ADDRINFO hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

	result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resulting_address);
	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return 1;
	}

	listen_socket = socket(AF_INET,      // IPv4 address famly
		SOCK_STREAM,  // stream socket
		IPPROTO_TCP); // TCP

	if (listen_socket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resulting_address);
		WSACleanup();
		return 1;
	}

	result = bind(listen_socket, resulting_address->ai_addr, (int)resulting_address->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resulting_address);
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(resulting_address);

	result = listen(listen_socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	printf("Server initialized, waiting for clients.\n");


	char* message;
	int length = 0;
	unsigned long mode = 1;
	result = ioctlsocket(listen_socket, FIONBIO, &mode);
	if (result != NO_ERROR)
	{
		printf("ioctlsocket za listen failed with error: %ld\n", result);
	}
	result = listen(listen_socket, SOMAXCONN);


	HANDLE accepted_thread[CLIENTS_NO];
	DWORD ids[CLIENTS_NO];
	int counter = 0;

	while (1)
	{
		if (counter != CLIENTS_NO)
		{
			Base_custom_select(listen_socket, 'r');
		}
		else
			break;

		struct sockaddr_in client_addr;
		int client_addr_size = sizeof(struct sockaddr_in);
		accepted_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_addr_size);

		if (accepted_socket == INVALID_SOCKET)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(listen_socket);
			WSACleanup();
			return 1;
		}
		else
		{
			if (ioctlsocket(accepted_socket, FIONBIO, &mode) != 0)
			{
				printf("ioctlsocket failed with error.");
				continue;
			}
			printf("New client request accepted (%d). Client address: %s : %d\n", counter, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));


			accepted_thread[counter] = CreateThread(NULL, 0, &worker_thread_function, accepted_socket, 0, &ids[counter]);

			counter++;
		}
	}


	for (int i = 0; i < CLIENTS_NO; i++) {
		WaitForSingleObject(accepted_thread[counter], INFINITE);
	}

	getchar();

	result = shutdown(listen_socket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(accepted_socket);
		WSACleanup();
		return 1;
	}


	for (int i = 0; i < CLIENTS_NO; i++) {
		CloseHandle(accepted_thread[i]);
	}



	DeleteCriticalSection(&cs);
	ManagerInitialization_destroy_manager();
	closesocket(listen_socket);
	closesocket(accepted_socket);
	WSACleanup();

	return 0;
}

DWORD WINAPI worker_thread_function(LPVOID param)
{

	SOCKET client_socket = (SOCKET)param;
	char* message;

	Base_custom_select(client_socket, 'r');
	int bytes_recieved = Base_custom_recieve(client_socket, &message);
	//printf("Primio od klijenta: %s\n", message);

	clock_t start_time, end_time;
	double cpu_time_used;
	start_time = clock();

	char* message_for_advanced_malloc = advanced_malloc(bytes_recieved);
	advanced_free(message_for_advanced_malloc);

	end_time = clock();
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

	printf("Vreme potrebno za zauzimanje %d bajtova advanced malloc i free je : %f\n", bytes_recieved, cpu_time_used);

	free(message);

	//vracanje poruke nasumicne duzine
	int length = Data_generate_message(&message, 20);
	Base_custom_select(client_socket, 'w');
	Base_custom_send(client_socket, message, length);
	closesocket(client_socket);
	free(message);
}
