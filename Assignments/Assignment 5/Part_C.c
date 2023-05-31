//
// Created by alon on 5/31/23.
//
#include "Part_C.h"

void cleanupHandler(void * vao) {
    // Cleanup code here
    AO * ao = (AO*)vao;
    destroyQueue(ao->queue);
    free(ao);
}

int threadFunction(AO *ao) {
    pthread_cleanup_push(cleanupHandler, ao) ;

            while (1) {
                // check for cancel
                pthread_testcancel();
                void *v = dequeue(ao->queue);
                int *pnum = (int *) v;
                ao->func(*pnum);
            }
    pthread_cleanup_pop(1);
}

AO *CreateActiveObject(int (*func)(int)) {
    AO *ao = malloc(sizeof(AO));
    initializeQueue(ao->queue);
    ao->func = func;
    pthread_create(&(ao->thread), NULL, (void *(*)(void *)) threadFunction, ao);

    return ao;
}