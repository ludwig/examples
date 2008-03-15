/* Copyright 2006 David Crawshaw, released under the new BSD license.
 * Version 2, from http://www.zentus.com/c/hash.html */

#ifndef __HASH__
#define __HASH__

/* Opaque structure used to represent hashtable. */
typedef struct hash hash;

/* Create new hashtable. */
hash * hash_new(unsigned int size);

/* Free hashtable. */
void hash_destroy(hash *h);

/* Add key/value pair. Returns non-zero value on error (eg out-of-memory). */
int hash_add(hash *h, const char *key, void *value);

/* Return value matching given key. */
void * hash_get(hash *h, const char *key);

/* Remove key from table, returning value. */
void * hash_remove(hash *h, const char *key);

/* Returns total number of keys in the hashtable. */
unsigned int hash_size(hash *h);

#endif
