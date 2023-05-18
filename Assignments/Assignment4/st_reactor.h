//
// Created by alon on 5/18/23.
//

#ifndef ASSIGNMENT4_ST_REACTOR_H
#define ASSIGNMENT4_ST_REACTOR_H


typedef int (*handler_t)(int fd, ...) ;

void * createReactor();

void stopReactor(void * this);

void startReactor(void* this);

void addFd(void * this,int tfd, handler_t handler);

void WaitFor(void * this);

#endif //ASSIGNMENT4_ST_REACTOR_H