//
// Created by alon on 5/31/23.
//

#ifndef ASSIGNMENT_5_PART_C_H
#define ASSIGNMENT_5_PART_C_H

#include "queue.h"

struct AO;
typedef struct param{
    struct AO* this;
    struct AO * next;
    int N;
    int seed;
    int * flag;

} param,*pparam;

typedef struct AO{
    pthread_t thread;
    Queue * queue;
    void (*func)(int,pparam);
    Queue* (*getQueue)(struct AO * this);
    void (*stop)(struct AO this);

}AO;


void threadFunction(void *);
AO *CreateActiveObject(void (*func)(int, pparam),pparam p);
void cleanupHandler(void * vao);


#endif //ASSIGNMENT_5_PART_C_H
