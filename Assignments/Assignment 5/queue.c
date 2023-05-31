//
// Created by alon on 5/31/23.
//

// from gpt. request 1: write for me a threads safe queue in C. the queue should hold void*.
//           request 2: now i want u to improve  this queue: the new queue will use cond to
//                     let the threads that try to dequeue not wait on busy loop( if the queue
//                     is empty or some other thread working on the queue, the thread should
//                     sleep until it can dequeue)
//          request 3: give me the header of this file.
#include "queue.h"


void initializeQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

bool isEmpty(Queue* queue) {
    return queue->front == NULL;
}

void enqueue(Queue* queue, void* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

void* dequeue(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    while (isEmpty(queue)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    Node* frontNode = queue->front;
    void* data = frontNode->data;

    queue->front = queue->front->next;
    free(frontNode);

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    pthread_mutex_unlock(&queue->mutex);

    return data;
}

void destroyQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }

    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->mutex);
}
