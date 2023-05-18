

#ifndef HS
#define HS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {

    // key is string
    int key;

    // value is also string
    void* value;
    struct node* next;
} node, * pnode;

typedef struct hashMap {

    // Current number of elements in hashMap
    // and capacity of hashMap
    int numOfElements, capacity;

    // hold base address array of linked list
    struct node** arr;
} hashMap, * PhashMap;

void setNode(struct node* node, int key , void* value);
void initializeHashMap(struct hashMap* mp);
void increase_HashMap(struct hashMap* mp,int new_capacity);
int hashFunction(struct hashMap* mp, int key);
void insert(struct hashMap* mp, int key, char* value);
void delete (struct hashMap* mp, int key);
void* search(struct hashMap* mp, int key);



#endif