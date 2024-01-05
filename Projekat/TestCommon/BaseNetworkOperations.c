#include "BaseNetworkOperations.h"

BOOL Base_initialize_windows_sockets()
{
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

void Base_custom_select(SOCKET s, char operation) {
	if (operation == 'r') {
		//ovo je citanje
		do {
			FD_SET set;
			TIMEVAL time_val;
			FD_ZERO(&set);
			FD_SET(s, &set);
			time_val.tv_sec = 1;
			time_val.tv_usec = 0;
			int result = select(0 /* ignored */, &set, NULL, NULL, &time_val);

			if (result > 0)
			{
				//Sleep(200);
				break;
			}

		} while (1);
	}
	else if (operation == 'w') {
		//ovo je pisanje
		do {
			FD_SET set;
			TIMEVAL time_val;
			FD_ZERO(&set);
			FD_SET(s, &set);
			time_val.tv_sec = 1;
			time_val.tv_usec = 0;

			int result = select(0 /* ignored */, NULL, &set, NULL, &time_val);

			if (result > 0)
			{
				break;
			}
		} while (1);
	}




}

void Base_custom_send(SOCKET s, char* niz, int broj_bajtova) {
	int result = 0;
	int bytes_sent = 0;

	Base_custom_select(s, 'w');
	result = send(s, (char*)&broj_bajtova, 4, NULL);

	do {
		Base_custom_select(s, 'w');
		result = send(s, niz + bytes_sent, broj_bajtova - bytes_sent, NULL);

		if (result == 0) {

			continue;
		}
		else if (result == -1) {
			printf("\nGreska!\n");
			return;
		}

		bytes_sent += result;

	} while (bytes_sent < broj_bajtova);
}

int Base_custom_recieve(SOCKET s, char** niz) {

	int result = 0;
	int bytes_recieved = 0;

	char* buff = (char*)malloc(4 * sizeof(char));
	//u niz smestamo 4bajtni broj koji nam govori kolika je kolicina podataka koju treba da primimo kroz mrezu
	Base_custom_select(s, 'r');
	result = recv(s, buff, 4, NULL);

	//pocetnu adresu prihvatnog bafera kastujem na int* (4 bajta) i dereferenciram kako bih dobio kolicinu podatka
	int bytes_to_recieve = *((int*)buff);
	free(buff);
	*niz = (char*)malloc(bytes_to_recieve);

	do {
		Base_custom_select(s, 'r');
		result = recv(s, *niz + bytes_recieved, bytes_to_recieve - bytes_recieved, NULL);

		if (result == 0) {
			continue;
		}
		else if (result == -1) {
			printf("\nGreska!\n");
			break;
		}

		bytes_recieved += result;

	} while (bytes_recieved < bytes_to_recieve);

	return bytes_to_recieve;
}
