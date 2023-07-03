//
// Created by alon on 5/18/23.
//

#ifndef ASSIGNMENT4_ST_REACTOR_H
#define ASSIGNMENT4_ST_REACTOR_H
#include "hash.h"
#include <pthread.h>


typedef struct reactor{
    PhashMap hash;
    int thread;

    void (* stopReactor)(void * this);
    void (*startReactor)(void* this);
    void (*addFd)(void * this,int tfd, handler_t handler);
    void (*WaitFor)(void * this);

} reactor, * preactor ;

void * createReactor();

void stopReactor(void * this);

void startReactor(void* this);

void addFd(void * this,int tfd, handler_t handler);

void WaitFor(void * this);

#endif //ASSIGNMENT4_ST_REACTOR_H