#ifndef HASH_MAP_<K,V>_H
#define HASH_MAP_<K,V>_H

/**
 * Generated file for a generic Hash_Map.
 */
#include <stdlib.h>

struct Hash_Map_<K,V>
{
	unsigned int capacity_log2;
	unsigned int (*hash)(K);
	K zero_key;
	K tombstone;
	K* keys;
	V* values;
};

struct Hash_Map_<K,V>* Hash_Map_<K,V>_init(unsigned int capacity, unsigned int (*hash)(K), K zero_key, K tombstone)
{
	unsigned int actual_capacity = 0b1_0000;
	unsigned int actual_capacity_log2 = 4;
	while (actual_capacity < capacity)
	{
		actual_capacity = actual_capacity << 1;
		actual_capacity_log2++;
	}

	K* keys = (K*)calloc(actual_capacity, sizeof(K));
	V* values = (V*)calloc(actual_capacity, sizeof(V));
	struct Hash_Map_<K,V>* retval = (struct Hash_Map_<K,V>*)malloc(sizeof(struct Hash_Map_<K,V>));
	if (keys != NULL)
	{
		for (int idx = 0; idx < actual_capacity; idx++)
			keys[idx] = zero_key;
	}
	if (retval != NULL)
	{
		retval->capacity_log2 = actual_capacity_log2;
		retval->hash = hash;
		retval->zero_key = zero_key;
		retval->tombstone = tombstone;
		retval->keys = keys;
		retval->values = values;
	}

	if (keys == NULL || values == NULL || retval == NULL)
	{
		free(keys);
		free(values);
		free(retval);
		retval = NULL;
	}
	return retval;
}

#endif
