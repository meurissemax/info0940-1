/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

#ifndef kvs_h
#define kvs_h

#include <linux/list.h> 

// Pointer to the head of the linked list
static LIST_HEAD(kvsHead);

// Structure defining a node in the linked list, containing an entry of the KVS
typedef struct Node {
    char* key;
    char* val;
    size_t keylen;
    size_t vallen;

    struct list_head kvsList;
} Node;

/**
 * Inserts an entry in the KVS.
 *
 * @param 	key 		pointer to a key string to be stored.
 * @param 	keylen  	length of the key string.
 * @param   val         pointer to a value string to be stored.
 * @param   vallen      length of the value string.
 * 
 * @return  0 on success, -1 on failure.						
 */
asmlinkage long kvs_insert(const char *key, size_t keylen, const char *val, size_t vallen);

/**
 * Searches for an entry in the KVS.
 *
 * @param 	key 		pointer to a key string to be searched.
 * @param 	keylen  	length of the key string.
 * @param   val         pointer to a buffer to receive the result value.
 * @param   vallen      size of the 'val' buffer
 * 
 * @return  1 if a value is found for a specified key, 0 if not, -1 on failure.
 * 
 * NB : If the buffer is not big enough to receive the value, -1 is returned.		
 */
asmlinkage long kvs_search(const char *key, size_t keylen, char *val, size_t max_vallen);

/**
 * Deletes an entry in the KVS.
 *
 * @param 	key 		pointer to a key string to be deleted.
 * @param 	keylen  	length of the key string.
 * 
 * @return  0 on success (deletion or nothing found for the given key),
 *          -1 on failure.						
 */
asmlinkage long kvs_delete(const char *key, size_t keylen);

#endif
