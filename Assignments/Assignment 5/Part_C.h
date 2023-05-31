//
// Created by alon on 5/31/23.
//

#ifndef ASSIGNMENT_5_PART_C_H
#define ASSIGNMENT_5_PART_C_H

#include "queue.h"

typedef struct AO{
    pthread_t thread;
    Queue * queue;
    int (*func)(int);
    Queue* (*getQueue)(struct AO this);
    void (*stop)(struct AO this);

}AO;

int threadFunction(AO *ao);
AO *CreateActiveObject(int (*func)(int));
void cleanupHandler(void * vao);


#endif //ASSIGNMENT_5_PART_C_H
