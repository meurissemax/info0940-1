/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include "kvs.h"

asmlinkage long kvs_insert(const char *key, size_t keylen, const char *val, size_t vallen){
    // Variables declaration (C90 compliant).
    Node* datastruct;
    Node* addition;

    // Check if key and val are allocated
    if(!val){
        printk(KERN_ERR "Insertion failed. The string 'val' was not allocated.\n");
        return -1;
    }
    if(!key){
        printk(KERN_ERR "Insertion failed. The key 'key' was not allocated.\n");
        return -1;
    }

    // 1. The key already exists in the KVS. We only update its field.

    // Create a Node to iterate the linked list.
    datastruct = NULL;

    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // Key may already exist
        if(strncmp(datastruct->key, key, keylen) == 0){
            // Only the prefix of datastruct->keylen matched, so no match for this entry
            if(keylen != datastruct->keylen)
                continue;

            // Free the field and reallocate it at the right size
            kfree(datastruct->val);
            datastruct->val = (char*) kmalloc(vallen*sizeof(char), GFP_KERNEL);

            // The allocation failed. Delete the entry.
            if(!datastruct->val){
                printk(KERN_ERR "Allocation of 'val' failed.\n");
                // Update the linked list
                list_del(&datastruct->kvsList);

                // free the Node
                kfree(datastruct->key);
                kfree(datastruct);
                return -1;
            }

            // No error in allocation, copy vallen characters of val into the field
            strncpy(datastruct->val, val, vallen);
            datastruct->vallen = vallen;
            return 0;
        }
    }

    // 2. We have to create a new node

    addition = (Node*) kmalloc(sizeof(Node),GFP_KERNEL);

    if(!addition){
        printk(KERN_ERR "Unable to create the entry. Memory may be full.\n");
        return -1;
    }

    addition->key = (char*) kmalloc(keylen*sizeof(char), GFP_KERNEL);

    if(!addition->key){
        printk(KERN_ERR "Unable to create the entry. The allocation for the key has not succeeded.\n");
        kfree(addition);
        return -1;
    }

    addition->val = (char*) kmalloc(vallen*sizeof(char), GFP_KERNEL);

    if(!addition->val){
        printk(KERN_ERR "Unable to create the entry. The allocation for the value has not succeeded.\n");
        kfree(addition->key);
        kfree(addition);
        return -1;
    }

    // Copy the arguments in the fields.

    addition->keylen = keylen;
    addition->vallen = vallen;
    strncpy(addition->val, val, vallen);
    strncpy(addition->key, key, keylen);

    INIT_LIST_HEAD(&addition->kvsList);

    // Add the node in the linked list.
    list_add(&addition->kvsList, &kvsHead);
    
    return 0;
}


asmlinkage long kvs_search(const char *key, size_t keylen, char *val, size_t max_vallen) {

    Node* datastruct;

    if(!val){
        printk("Search failed. The buffer 'val' was not allocated.\n");
        return -1;
    }
    if(!key){
        printk("Search failed. The key 'key' was not allocated.\n");
        return -1;
    }

    // Create a Node to iterate the linked list.
    datastruct = NULL;
    
    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // We may have the key in KVS.
        if(strncmp(datastruct->key, key, keylen)==0){
            // Only the prefix of datastruct->keylen matched, so no match for this entry
            if(keylen != datastruct->keylen)
                continue;

            // 1. Buffer large enough
            if(max_vallen >= datastruct->vallen){
                strncpy(val, datastruct->val, datastruct->vallen); 
                return 1;
            }
            
            // 2. The buffer is not large enough. We decided not to copy anything in it and return -1.
            else{
                printk(KERN_ERR "Key found but buffer not large enough to contain the value.\n");
                return -1;
            }
        }
    }
    return 0;
}


asmlinkage long kvs_delete(const char *key, size_t keylen) {

    Node* datastruct;

    // The pointer key is not allocated
    if(! key){
        printk(KERN_ERR "key pointer not allocated.\n");
        return -1;
    }

    // Create a Node to iterate the linked list.
    datastruct = NULL;
    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // We may have found the key
        if(strncmp(datastruct->key, key, keylen)==0){
            // Only the prefix of datastruct->keylen matched, so no match for this entry
            if(keylen != datastruct->keylen)
                continue;

            // Update the linked list
            list_del(&datastruct->kvsList);

            // free the Node
            kfree(datastruct->key);
            kfree(datastruct->val);
            kfree(datastruct);
            return 0;
        }
    }
    // No key found, still return 0
    return 0;
}
