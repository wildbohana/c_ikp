#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
*	Data_generate_message() - Generise poruku nasumicne duzine, vraca duzinu poruke
*
*	message			: pokazivac na niz karaktera u koji ce se smestiti generisana poruka
*	random_length	: celobrojna vrednost na osnovu koje se generise duzina poruke
**/
int Data_generate_message(char** message, int random_length);
