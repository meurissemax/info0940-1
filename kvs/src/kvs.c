/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

/***********/
/* Headers */
/***********/

#include <linux/kernel.h>
#include <linux/types.h>


/*************/
/* Functions */
/*************/

asmlinkage long kvs_insert(const char* key, size_t keylen, const char* val, size_t vallen) {
	printk("KVS insert.\n");

	return 0;
}

asmlinkage long kvs_search(const char* key, size_t keylen, char* val, size_t max_vallen) {
	printk("KVS search.\n");

	return 0;
}

asmlinkage long kvs_delete(const char* key, size_t keylen) {
	printk("KVS delete.\n");

	return 0;
}
