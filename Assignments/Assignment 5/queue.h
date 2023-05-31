//
// Created by alon on 5/31/23.
//

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

typedef struct {
    Node* front;
    Node* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

void initializeQueue(Queue* queue);
bool isEmpty(Queue* queue);
void enqueue(Queue* queue, void* data);
void* dequeue(Queue* queue);
void destroyQueue(Queue* queue);

#endif //ASSIGNMENT_5_QUEUE_H
