//
// Created by alon on 5/31/23.
//
#include "Part_C.h"

Queue * getQueue(struct AO this){
    return this.queue;
}
void stop(struct AO this){
    pthread_cancel(this.thread); // ask the thread to stop
    pthread_join(this.thread, NULL); // wait until the thread will stop
}

void cleanupHandler(void * vao) {
    // Cleanup code here
    AO * ao = (AO*)vao;
    destroyQueue(ao->queue);
    free(ao);
}

void threadFunction(void * p2) {
    pparam p = (pparam) p2;
    AO * this = p->this;
    pthread_cleanup_push(cleanupHandler, this) ;

            while (1) {
                // check for cancel
                pthread_testcancel();
                void *v = dequeue(this->queue);
                int *pnum = (int *) v;
                this->func(*pnum, p);
            }
    pthread_cleanup_pop(1);
}

AO *CreateActiveObject(void (*func)(int,pparam),pparam p) {
    AO *ao = malloc(sizeof(AO));
    initializeQueue(ao->queue);
    ao->func = func;
    p->this = ao;
    pthread_create(&(ao->thread), NULL, (void *(*)(void *)) threadFunction, &p);

    return ao;
}