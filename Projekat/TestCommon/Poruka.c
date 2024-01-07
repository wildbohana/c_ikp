#include "Poruka.h"

// Generisanje poruke
int Generisi_Poruku(char** poruka, int random_duzina) {
	// 0-500 * 200 000 = do 100 000 000 bajta (100 MB)
	// Prvi +1 da nikad ne bude 0, drugi +1 za NULL terminator
	int duzina = (random_duzina * 200000) + 1 + 1;

	*poruka = malloc(sizeof(char) * duzina);
	for (int i = 0; i < duzina - 1; i++) {
		(*poruka)[i] = '0' + (i % 10);
	}

	// NULL na kraj poruke (\0)
	(*poruka)[duzina - 1] = 0;

	return duzina;
}
