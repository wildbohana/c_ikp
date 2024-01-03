#pragma once
#include "Structs.h"
#include "..\HeapManager\HeapCreation.h"
#include "..\HeapManager\HeapDestruction.h"
#include "..\HeapManager\HeapManipulation.h"

// Recnik sa heap-ovima
extern Dictionary* _dictionary;

BOOL _Dictionary_create(int minimal_size);
BOOL _Dictionary_insert(void* key, void* value);
BOOL _Dictionary_remove(void* key, void** value);
void _Dictionary_destroy();
