#pragma once
#include <stdlib.h>
#include "DataSerializationOperations.h"
#include "stdafx.h"


void Data_serialize(pak* head, zpak** serialized_data, int n) {
	*serialized_data = (zpak*)malloc(sizeof(zpak) * n);

	for (int i = 0; i < n; i++) {
		(*serialized_data)[i].data = head->data;
		(*serialized_data)[i].size = head->size;
		head = head->next;
	}
}

void Data_deserialize(pak** head, pak** tail, char* niz, int number_of_bytes) {
	zpak* packet_array = (zpak*)niz;

	for (int i = 0; i < (number_of_bytes / (int)sizeof(zpak)); i++) {
		pak* p = (pak*)malloc(sizeof(pak));
		p->data = packet_array[i].data;
		p->next = NULL;

		if (*head == NULL) {
			*head = *tail = p;
			continue;
		}

		(*tail)->next = p;
		*tail = p;
	}

	return;
}
