#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 10

typedef struct Hashmap {
    char* key;
    int value;
    struct Hashmap* next;
    } Hashmap;

    unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = str++)){
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

void hashmap_put(Hashmap** map, char* key, int value) {
    unsigned long hash_val = hash(key) % HASHMAP_SIZE;
    Hashmap* new_entry = (Hashmap*) malloc(sizeof(Hashmap));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = map[hash_val];
    map[hash_val] = new_entry;
}

int hashmap_get(Hashmap** map, char* key) {
    unsigned long hash_val = hash(key) % HASHMAP_SIZE;
    Hashmap* entry = map[hash_val];
    while (entry != NULL) {
    if (strcmp(entry->key, key) == 0)
    return entry->value;
    entry = entry->next;
    }
return -1;
}

int main() {
    Hashmap* map[HASHMAP_SIZE];
    memset(map, 0, sizeof(map));

    hashmap_put(map, "key1", 1);
    hashmap_put(map, "key2", 2);
    hashmap_put(map, "key3", 3);
    hashmap_put(map, "key4", 4);

    printf("Value for key 'key1': %d\n", hashmap_get(map, "key1"));
    printf("Value for key 'key2': %d\n", hashmap_get(map, "key2"));
    printf("Value for key 'key3': %d\n", hashmap_get(map, "key3"));
    printf("Value for key 'key4': %d\n", hashmap_get(map, "key4"));
}