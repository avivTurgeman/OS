//
// Created by alon on 5/18/23.
//
/*
 * base code copied from https://www.geeksforgeeks.org/implementation-of-hash-table-in-c-using-separate-chaining/
 * and changed to fit for us
 */
#include "hash.h"

// like constructor
void setNode(struct node* node, int key , handler_t value)
{
    node->key = key;
    node->value = value;
    node->next = NULL;
};


// like constructor
void initializeHashMap(struct hashMap* mp)
{

    // Default capacity in this case
    mp->capacity = 100;
    mp->numOfElements = 0;

    // array of size = 1
    mp->arr = (struct node**)malloc(sizeof(struct node*)
                                    * mp->capacity);

}

void increase_HashMap(struct hashMap* mp,int new_capacity){
    struct node ** new_arr = realloc(mp->arr,sizeof(struct node*) * new_capacity);
    free(mp->arr);
    mp->arr = new_arr;
}

int hashFunction(struct hashMap* mp, int key)
{
    int bucketIndex;
    int sum = 0, factor = 31;
    for (int i = 0; i < key; i++) {

        sum = ((sum % mp->capacity)
               + (key * factor) % mp->capacity)
              % mp->capacity;

        // factor = factor * prime
        // number....(prime
        // number) ^ x
        factor = ((factor % __INT16_MAX__)
                  * (31 % __INT16_MAX__))
                 % __INT16_MAX__;
    }

    bucketIndex = sum;
    return bucketIndex;
}

void insert(struct hashMap* mp, int key, handler_t value)
{

    // Getting bucket index for the given
    // key - value pair
    int bucketIndex = hashFunction(mp, key);
    // Creating a new node
    struct node* newNode = (struct node*)malloc(sizeof(struct node));



    // Setting value of node
    setNode(newNode, key, value);

    // Bucket index is empty....no collision
    if (mp->arr[bucketIndex] == NULL) {
        mp->arr[bucketIndex] = newNode;
    }

        // Collision
    else {

        // Adding newNode at the head of
        // linked list which is present
        // at bucket index....insertion at
        // head in linked list
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
}

void delete (struct hashMap* mp, int key)
{

    // Getting bucket index for the
    // given key
    int bucketIndex = hashFunction(mp, key);

    struct node* prevNode = NULL;

    // Points to the head of
    // linked list present at
    // bucket index
    struct node* currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {

        // Key is matched at delete this
        // node from linked list
        if ( key == currNode->key ) {

            // Head node
            // deletion
            if (currNode == mp->arr[bucketIndex]) {
                mp->arr[bucketIndex] = currNode->next;
            }

                // Last node or middle node
            else {
                prevNode->next = currNode->next;
            }
            free(currNode);
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
}

handler_t search(struct hashMap* mp, int key)
{

    // Getting the bucket index
    // for the given key
    int bucketIndex = hashFunction(mp, key);

    // Head of the linked list
    // present at bucket index
    struct node* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {

        // Key is found in the hashMap
        if (bucketHead->key == key) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }

    // If no key found in the hashMap
    // equal to the given key
    // returning NULL
    return NULL;
}

