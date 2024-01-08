#include "HashTable.h"

// Pomocna funkcija, racuna (a^b)%c
int _HashTable_modulo(int a, int b, int c) {
	long long x = 1, y = a;	// long long jer moze doci do overflow-a int-a
	while (b > 0) {
		if (b % 2 == 1) {
			x = (x * y) % c;
		}
		y = (y * y) % c;	// kvadriranje baze
		b /= 2;
	}
	return x % c;
}

// Pomocna funkcija, racuna (a^b)%c i uzima u obzir da a^b moze da overflow-uje 
long long _HashTable_mulmod(long long a, long long b, long long c) {
	long long x = 0, y = a % c;
	while (b > 0) {
		if (b % 2 == 1) {
			x = (x + y) % c;
		}
		y = (y * 2) % c;
		b /= 2;
	}
	return x % c;
}

// Pomocna funkcija koja koristi miller-rabin-ovu metodu za racunanje sledeceg prostog broja
BOOL _HashTable_miller_rabin(long long p, int iteration) {
	if (p < 2) {
		return FALSE;
	}

	if (p != 2 && p % 2 == 0) {
		return FALSE;
	}
	
	long long s = p - 1;
	while (s % 2 == 0) {
		s /= 2;
	}
	for (int i = 0; i < iteration; i++) {
		long long a = rand() % (p - 1) + 1, temp = s;
		long long mod = _HashTable_modulo(a, temp, p);

		while (temp != p - 1 && mod != 1 && mod != p - 1) {
			mod = _HashTable_mulmod(mod, mod, p);
			temp *= 2;
		}
		
		if (mod != p - 1 && temp % 2 == 0) {
			return FALSE;
		}
	}
	return TRUE;
}

// Hash algoritam koji odredjuje hash uz pomoc multiplikacije i siftovanja
inline uint32_t _HashTable_get_hash(void* key) {
	uintptr_t a = (uintptr_t)key;
	a -= (a << 6);
	a ^= (a >> 17);
	a -= (a << 9);
	a ^= (a << 4);
	a -= (a << 3);
	a ^= (a << 10);
	a ^= (a >> 15);
	
	// U slucaju 64-bit adresa, siftuju se najznacajniji biti (koji su najvazniji), kako bi se dobio 32-bitni broj
	return a >> ((sizeof(uintptr_t) - sizeof(uint32_t)) * 8);
}

// Inicijalizacija HashTable
BOOL HashTable_inicijalizacija_tabele(HashTable* tabela, unsigned int minimalna_velicina, void* (*bucket_list_allocating_function)(int), void(*bucket_list_free_function)(HashNode**), void* (*node_allocate_function)(), void(*node_free_function)(HashNode*)) {
	BOOL ret = TRUE;

	tabela->trenutno_elemenata = 0;

	// Osiguraj da prosledjeni parametri nisu NULL
	if (tabela == NULL)
		return FALSE;

	if (bucket_list_allocating_function != NULL)
		tabela->funkcija_za_alokaciju_bucketa = bucket_list_allocating_function;
	else
		return FALSE;

	if (bucket_list_free_function != NULL)
		tabela->funkcija_za_dealokaciju_bucketa = bucket_list_free_function;
	else
		return FALSE;

	if (node_allocate_function != NULL)
		tabela->funkcija_za_alokaciju_cvorova = node_allocate_function;
	else
		return FALSE;

	if (node_free_function != NULL)
		tabela->funkcija_za_dealokaciju_cvorova = node_free_function;
	else
		return FALSE;

	// Osiguraj da je minimalna velicina tabele prost broj
	if (minimalna_velicina > 0) {
		int size = minimalna_velicina;
		if (size % 2 == 0)
			size++;

		for (; size < 2 * size; size += 2)
			if (_HashTable_miller_rabin(size, 20))	// 20 iteracija
				break;
		tabela->trenutna_velicina = size;
	}
	else {
		tabela->trenutna_velicina = 1009;
	}
	tabela->minimalna_velicina = tabela->trenutna_velicina;
	
	// Alociraj tabelu i inicijalizuj je sa NULL pokazivacima
	tabela->bucketi = tabela->funkcija_za_alokaciju_bucketa(tabela->trenutna_velicina);
	if (tabela->bucketi != NULL)
		for (int i = 0; i < minimalna_velicina; i++)
			tabela->bucketi[i] = NULL;
	else
		ret = FALSE;

	return ret;
}

// Dobavljanje pokazivaca na HashNode sa prosledjenim kljucem
HashNode* HashTable_dobavi_element(HashTable* table, void* key) {
	if (table != NULL && key != NULL && table->bucketi != NULL) {
		// Pretvori kljuc u hash kako bi pronasla bucket sa elementima
		uint32_t index = _HashTable_get_hash(key) % table->trenutna_velicina;

		// Iteriraj kroz bucket u kom se nalazi element, i u slucaju pronalaska kljuca vrati pokazivac na element
		for (HashNode* node = table->bucketi[index]; node != NULL; node = node->sledeci) {
			if (node->kljuc == key)
				return node;
		}
		return NULL;
	}
	else {
		return NULL;

	}
}

// Funkcija koja menja velicinu tabele
// Ako je previse iskoriscena, udvostruci je
// Ako je premalo iskoriscena, prepolovi je
void _HashTable_rebuild_table(HashTable* tabela, BOOL povecaj) {
	HashNode** old_table, *next, *current;
	unsigned int old_size, index, i;
	float table_direction = 0;

	old_table = tabela->bucketi;
	old_size = tabela->trenutna_velicina;
	int new_size = 0;

	if (povecaj) {
		new_size = old_size << 1;
		table_direction = 2;
	}
	else {
		new_size = old_size >> 1;
		if (new_size < tabela->minimalna_velicina)
			new_size = tabela->minimalna_velicina;
		table_direction = 0.5;
	}

	// Nova velicina mora biti prost broj
	if (new_size % 2 == 0)
		new_size++;

	for (; new_size < 2 * table_direction * old_size; new_size += 2)
		if (_HashTable_miller_rabin(new_size, 20))
			break;

	tabela->trenutna_velicina = new_size;
	tabela->bucketi = tabela->funkcija_za_alokaciju_bucketa(tabela->trenutna_velicina);
	if (tabela->bucketi == NULL) {
		tabela->bucketi = old_table;
		tabela->trenutna_velicina = old_size;
		return;
	}

	for (int i = 0; i < tabela->trenutna_velicina; i++) {
		tabela->bucketi[i] = NULL;
	}

	for (i = 0; i < old_size; i++) {
		next = old_table[i];
		while (next) {
			current = next;
			next = next->sledeci;
			index = _HashTable_get_hash(current->kljuc) % tabela->trenutna_velicina;
			current->sledeci = tabela->bucketi[index];
			tabela->bucketi[index] = current;
		}
	}

	tabela->funkcija_za_dealokaciju_bucketa(old_table);
}

// Dodavanje novog elementa u HashTable
BOOL HashTable_ubaci_element(HashTable* table, void* key, void* value) {
	if (table != NULL && key != NULL) {
		// U slucaju da tabela vec sadrzi element sa datim kljucem, operacija vraca FALSE
		if (HashTable_dobavi_element(table, key) == NULL) {
			// U slucaju da je tabela popunjena vise od 75%, zbog performansa joj je potrebno udvostruciti velicinu
			if (table->trenutno_elemenata >= table->trenutna_velicina * 0.75)
				_HashTable_rebuild_table(table, TRUE);

			if (table->bucketi == NULL)
				return FALSE;

			// Pretvori kljuc u hash (indeks)
			uint32_t index = _HashTable_get_hash(key) % table->trenutna_velicina;
			// Alociraj memoriju za element
			HashNode* node = (HashNode*)table->funkcija_za_alokaciju_cvorova();
			// Alociranom elementu dodeli vrednosti
			node->kljuc = key;
			node->vrednost = value;
			node->sledeci = table->bucketi[index];

			// Alocirani element ubaci u tabelu
			if (table->bucketi != NULL) {
				table->bucketi[index] = node;
				table->trenutno_elemenata++;
			}

			return TRUE;
		}
		else
			return FALSE;
	}
	else {
		return FALSE;
	}
}

// Brisanje elementa iz tabele
BOOL HashTable_izbaci_element(HashTable* table, void* key, void** out_value) {
	if (table != NULL && key != NULL) {
		// Dobavi element sa datim kljucem
		HashNode* node = HashTable_dobavi_element(table, key);

		if (node != NULL) {
			// Uz pomoc hash funkcije izracunaj index bucket-a elementa
			uint32_t index = _HashTable_get_hash(key) % table->trenutna_velicina;

			// Ako je element prvi cvor u bucket listi, samo prevezi cvorove
			if (node == table->bucketi[index]) {
				table->bucketi[index] = node->sledeci;
			}
			else {
				HashNode* last;
				// Pronadji element koji se nalazi pre elementa za brisanje
				for (last = table->bucketi[index]; last != NULL && last->sledeci != NULL; last = last->sledeci)
					if (last->sledeci == node)
						break;

				// Izbaci element za brisanje iz tabele (prevezi cvorove)
				last->sledeci = node->sledeci;
			}

			table->trenutno_elemenata--;
			*out_value = node->vrednost;
			table->funkcija_za_dealokaciju_cvorova(node);

			// Ako je iskoriscenost Heap-a <=25%, prepolovi Heap radi efikasnosti
			if (table->trenutna_velicina > table->minimalna_velicina && table->trenutno_elemenata <= table->trenutna_velicina * 0.25)
				_HashTable_rebuild_table(table, FALSE);

			return TRUE;
		}
		else
			return FALSE;
	}
	else {
		return FALSE;
	}
}

// Deinicijalizacija Hash tabele
BOOL HashTable_deinicijalizacija_tabele(HashTable* tabela) {
	if (tabela != NULL) {
		HashNode* trenutni = NULL;
		HashNode* sledeci = NULL;

		// Prolazak kroz tabelu i oslobadjanje svih ulancanih elemenata
		for (int i = 0; i < tabela->trenutna_velicina; i++) {
			trenutni = tabela->bucketi[i];
			while (trenutni != NULL) {
				sledeci = trenutni->sledeci;
				tabela->funkcija_za_dealokaciju_cvorova(trenutni);
				trenutni = sledeci;
			}
		}

		// Oslobadjanje same tabele
		tabela->funkcija_za_dealokaciju_bucketa(tabela->bucketi);
		tabela->trenutna_velicina = 0;
		tabela->trenutno_elemenata = 0;
		return TRUE;
	}
	else {
		return FALSE;
	}
}
