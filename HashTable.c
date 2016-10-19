// HashTable.c ... implementation of HashTable ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"
#include "List.h"

// Types and functions local to HashTable ADT

typedef struct HashTabRep {
	List *lists;  // either use this
	int   nslots; // # elements in array
	int   nitems; // # items stored in HashTable
} HashTabRep;

// convert key into index (from Sedgewick)
unsigned int hash(Key k, int tableSize)
{
	unsigned int h = 0;
	int a = 31415, b = 27183;
	for (; *k != '\0'; k++) {
		a = a*b % (tableSize-1);
		h = (a*h + *k) % tableSize;
	}
	return (h % tableSize);
}


// Interface functions for HashTable ADT

// create an empty HashTable
HashTable newHashTable(int N)
{
	HashTabRep *new = malloc(sizeof(HashTabRep));
	assert(new != NULL);
	new->lists = malloc(N*sizeof(List));
	assert(new->lists != NULL);
	int i;
	for (i = 0; i < N; i++)
		new->lists[i] = newList();
	new->nslots = N; new->nitems = 0;
	return new;
}

// free memory associated with HashTable
void dropHashTable(HashTable ht)
{
	assert(ht != NULL);
	int i;
    for (i = 0; i < ht->nslots; i++)
		dropList(ht->lists[i]);
	free(ht);
}

// display HashTable stats
void HashTableStats(HashTable ht)
{

	int slotnum = ht->nslots;
	int itemnum = ht->nitems;
	int i = 0;
	int *lengthIndex = malloc(sizeof(int)*ht->nitems);

	assert(ht != NULL);
	printf("Hash Table Stats:\n");
	printf("Number of slots = %d\n",slotnum); // TODO
	printf("Number of items = %d\n",itemnum); // TODO
	printf("Chain length distribution\n");
	printf("%8s %8s\n","Length","#Chains");
	// TODO .. rest of function to show length/freq pairs

	// create an array of size nitems (to hold max num of lengths)
	for (i = 0; i < itemnum; i++){
		lengthIndex[i] = 0;
	}

	int listLength = 0;
	for (i = 0; i < slotnum; i++) {
		listLength = listSize(ht->lists[i]);
		lengthIndex[listLength]++;
	}

	for (i = 0; i < itemnum; i++) {
		if (lengthIndex[i] != 0) {
			printf("%8d %8d\n", i, lengthIndex[i]);
		}
	}
	free(lengthIndex);
}

// insert a new value into a HashTable
void HashTableInsert(HashTable ht, Item it)
{
	assert(ht != NULL);
	int i = hash(key(it), ht->nslots);
	if (ListSearch(ht->lists[i], key(it)) == NULL) {
		ListInsert(ht->lists[i], it);
		ht->nitems++;
	}
}

// delete a value from a HashTable
void HashTableDelete(HashTable ht, Key k)
{
	assert(ht != NULL);
	int h = hash(k, ht->nslots);
	ListDelete(ht->lists[h], k);
}

// get Item from HashTable using Key
Item *HashTableSearch(HashTable ht, Key k)
{
	assert(ht != NULL);
	int i = hash(k, ht->nslots);
	return ListSearch(ht->lists[i], k);
}

