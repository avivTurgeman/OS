#ifndef ASSIGNMENT_5_QUEUE_H
#define ASSIGNMENT_5_QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool (*isEmpty)(struct Queue* queue);
    void (*enqueue)(struct Queue* queue, void* data);
    void* (*dequeue)(struct Queue* queue);
    void (*destroyQueue)(struct Queue* queue);
} Queue;

void initializeQueue(Queue* queue);
bool isEmpty(Queue* queue);
void enqueue(Queue* queue, void* data);
void* dequeue(Queue* queue);
void destroyQueue(Queue* queue);
void printQueue(Queue * queue);

#endif //ASSIGNMENT_5_QUEUE_H
