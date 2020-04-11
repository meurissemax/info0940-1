#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include "kvs.h"

// NB : For this assignment we do not consider the final '\0' character of string ending, 
// as it has been posted in the ecampus forum.

asmlinkage long kvs_insert(const char *key, size_t keylen, const char *val, size_t vallen){
    
    // 1. The key already exists in the KVS. We only update its field.

    // Create a Node to iterate the linked list.
    struct Node* datastruct = NULL;

    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // Key already exists
        if(strcmp(datastruct->key, key) == 0){
            kfree(datastruct->val);
            datastruct->val = (char*) kmalloc(vallen*sizeof(char), GFP_KERNEL);
            // The allocation failed
            if(!datastruct->val){
                // Error message to add
                return -1;
            }

            // No error in allocation, copy val into the field.

            // We cannot read outside of the allocated string val, so only fill 
            if(vallen > strlen(val)+1){
                // TO DO. Delete all 'for' for allocation
            }
            // No padding needed, vallen may even be smaller than strlen(val)+1
            else{
                int i;
                for(i = 0 ; i < vallen ; i++){
                    datastruct->val[i] = val[i];
                }
            }
            return 0;
        }
    }

    // 2. We have to create a new node

    struct Node* addition = (struct Node*) kmalloc(sizeof(struct Node),GFP_KERNEL);

    if(!addition){
        printk("Unable to create the entry. Memory may be full.\n"); // KERN_DEBUG ?
        return -1;
    }

    addition->key = (char*) kmalloc(keylen*sizeof(char), GFP_KERNEL);

    if(!addition->key){
        printk("Unable to create the entry. The allocation for the key has not succeeded.\n");
        kfree(addition);
        return -1;
    }

    addition->val = (char*) kmalloc(vallen*sizeof(char), GFP_KERNEL);

    if(!addition->val){
        printk("Unable to create the entry. The allocation for the value has not succeeded.\n");
        kfree(addition->key);
        kfree(addition);
        return -1;
    }

    // Copy the arguments in the fields.

    addition->keylen = keylen;
    addition->vallen = vallen;
    printk("here");
    // We cannot go read above the space allocated to the string, so padding
    if(keylen > strlen(key)+1){
        int i;
        int sizeKey = strlen(val) + 1;
                for(i = 0 ; i < sizeKey ; i++){
                    datastruct->key[i] = key[i];
                }
                for(i = sizeKey ; i < keylen ; i++){
                    datastruct->key[i] = '\0';
                }
    }
    // No padding required 
    else{
        printk("hoi");
        int i;
        for(i = 0 ; i < keylen ; i++){
            printk("soin");
            printk("%c",key[i]);
            printk("%c",datastruct->key[i]);
            datastruct->key[i] = key[i];
        }
    }
    printk("hoi");
    // The same is true for val
    if(vallen > strlen(val)+1){
        int i;
        int sizeVal = strlen(val) + 1;
        for(i = 0 ; i < sizeVal ; i++){
            datastruct->val[i] = val[i];
        }
        for(i = sizeVal ; i < vallen ; i++){
            datastruct->val[i] = '\0';
        }
    }
    else{
        int i;
        for(i = 0 ; i < vallen ; i++){
            datastruct->val[i] = val[i];
        }
    }

    INIT_LIST_HEAD(&addition->kvsList);
    
    /* Normally cannot happen
    if(!addition->kvsList){
        printk("Unable to create the entry.\n");
        kfree(addition->val);
        kfree(addition->key);
        kfree(addition);
        return -1;
    }*/
    printk("ip");
    // Add the node in the linked list.
    list_add(&addition->kvsList, &kvsHead);
    
    return 0;
}

asmlinkage long kvs_search(const char *key, size_t keylen, char *val, size_t max_vallen) {

    if(!val){
        printk("Search failed. The buffer 'val' was not allocated.\n");
        return -1;
    }
    if(!key){
        printk("Search failed. The key 'key' was not allocated.\n");
        return -1;
    }

    // Create a Node to iterate the linked list.
    struct Node* datastruct = NULL;
    
    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // We have the key in KVS.
        if(strcmp(datastruct->key, key)==0){
            // 1. The buffer is big enough
            if(max_vallen >= datastruct->vallen){ // Should we add the '\0' character ? +1 ?
                strncpy(val, datastruct->val, datastruct->vallen); // +1 ?
                return 1;
            }
            // 2. The buffer is not big enough.
            else{
                // What to do ?
                return -1;
            }
        }
    }
    return 0;
}

asmlinkage long kvs_delete(const char *key, size_t keylen) {

    // Create a Node to iterate the linked list.
    struct Node* datastruct = NULL;
    // Iterate over all entries in the list.
    list_for_each_entry(datastruct, &kvsHead, kvsList){
        // We find the key
        if(strcmp(datastruct->key, key)==0){
            list_del(&datastruct->kvsList);
            return 0;
        }
    }
    // Add error management
    return 0;
}