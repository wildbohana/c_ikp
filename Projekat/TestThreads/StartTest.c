#include "TestNiti.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Morate uneti broj heap-ova kao argument komandne linije!\n");
		return 0;
	}

	// Inicijalizacija Manager-a
	ManagerInitialization_inicijalizuj_manager(atoi(argv[1]));

	/* AHM FUNKCIJE TESTOVI */
	//test_ahm_init(1);
	//test_ahm_init(2);
	//test_ahm_init(5);
	//test_ahm_init(10);
	test_ahm_init(20);
	//test_ahm_init(50);

	// Brisanje Manager-a
	ManagerInitialization_deinicijalizuj_manager();

	/* UGRADJENE FUNKCIJE TESTOVI */
	//test_obican_init(1);
	//test_obican_init(2);
	//test_obican_init(5);
	//test_obican_init(10);
	test_obican_init(20);
	//test_obican_init(50);

	printf("\nPritisnite bilo sta za izlazak iz programa...\n");
	getchar();

	return 0;
}
