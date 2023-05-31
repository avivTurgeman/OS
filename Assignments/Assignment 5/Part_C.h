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
Queue * getQueue(struct AO this){
    return this.queue;
}
void stop(struct AO this){
    pthread_cancel(this.thread); // ask the thread to stop
    pthread_join(this.thread, NULL); // wait until the thread will stop
}


#endif //ASSIGNMENT_5_PART_C_H
