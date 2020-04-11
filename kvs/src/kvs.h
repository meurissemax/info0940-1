#include <linux/list.h> 

static LIST_HEAD(kvsHead);

struct Node {
    char* key;
    char* val;
    size_t keylen;
    size_t vallen;

    struct list_head kvsList;
};


asmlinkage long kvs_insert(const char *key, size_t keylen, const char *val, size_t vallen);

asmlinkage long kvs_search(const char *key, size_t keylen, char *val, size_t max_vallen);

asmlinkage long kvs_delete(const char *key, size_t keylen);
