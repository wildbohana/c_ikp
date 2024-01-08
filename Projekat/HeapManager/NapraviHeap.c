#include "NapraviHeap.h"

// Pomocna funkcija, podesavanja iz heapapi.h
// Poziva se nad vec kreiranim Heap-om
void static NapraviHeap_podesi_heap(Heap heap) {
	HEAP_OPTIMIZE_RESOURCES_INFORMATION HeapInfo;
	HeapInfo.Version = HEAP_OPTIMIZE_RESOURCES_CURRENT_VERSION;
	HeapInfo.Flags = 0;

	// HeapOptimizeResources - Heap brze decommit-uje memoriju, optimizuje potrosnju memorije procesa
	// HeapEnableTerminationOnCorruption - ugasi proces ako je doslo do korupcije heap-a
	HeapSetInformation(heap, HeapOptimizeResources, &HeapInfo, sizeof(HeapInfo));
	HeapSetInformation(heap, HeapEnableTerminationOnCorruption, NULL, 0); 
}

// Stvara ogranicen Heap
// Inicijalna velicina je jednaka 1/4 prosledjene vrednosti
// Maksimalna velicina Heap-a je jednaka prosledjenoj vrednosti
Heap NapraviHeap_ogranicen(int heap_size) {
	if (heap_size > 0) {
		Heap heap = HeapCreate(0, heap_size / 4, heap_size);
		if (heap != NULL) {
			NapraviHeap_podesi_heap(heap);
			return heap;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

// Stvara neogranicen Heap
// Inicijalna velicina je jednaka prosledjenoj vrednosti
// Maksimalna velicina Heap-a je 0, odnosno Heap moze da raste po potrebi
Heap NapraviHeap_neogranicen(int initial_commited_bytes) {
	if (initial_commited_bytes > 0) {
		Heap heap = HeapCreate(0, initial_commited_bytes, 0);
		if (heap != NULL) {
			NapraviHeap_podesi_heap(heap);
			return heap;
		}
		else {
			return NULL;
		}
	}
	else {
		// Ako je prosledjena velicina Heap-a manja od 0, napravi Heap od 50MB
		return HeapCreate(0, 50000000, 0);
	}
}

// Stvara neogranicen Heap koji nije "thead-safe"
// Inicijalna velicina je jednaka prosledjenoj vrednosti
// Maksimalna velicina Heap-a je 0, odnosno Heap moze da raste po potrebi
Heap NapraviHeap_neogranicen_unsafe(int initial_commited_bytes) {
	if (initial_commited_bytes > 0) {
		// HEAP_NO_SERIALIZE - A heap created with this option cannot be locked (thus - it's not thread-safe)
		Heap heap = HeapCreate(HEAP_NO_SERIALIZE, initial_commited_bytes, 0);
		if (heap != NULL) {
			NapraviHeap_podesi_heap(heap);
			return heap;
		}
		else {
			return NULL;
		}
	}
	else {
		// Ako je prosledjena velicina Heap-a manja od 0, napravi Heap od 50MB
		Heap heap = HeapCreate(HEAP_NO_SERIALIZE, 50000000, 0);
		if (heap != NULL) {
			NapraviHeap_podesi_heap(heap);
			return heap;
		}
		else { 
			return NULL; 
		}
	}
}
