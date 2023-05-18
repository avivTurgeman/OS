//
// Created by alon on 5/18/23.
//

#include "st_reactor.h"
void * createReactor(){
    preactor new_reactor = malloc(sizeof(reactor));
    new_reactor->thread = -1;

    new_reactor->WaitFor = WaitFor;
    new_reactor->addFd = addFd;
    new_reactor->startReactor = startReactor;
    new_reactor->stopReactor = stopReactor;

    PhashMap hashMap =  malloc(sizeof(hashMap));
    initializeHashMap(hashMap);
    new_reactor->hash = hashMap;

    return new_reactor;
}

void stopReactor(void * this){
    int t =((preactor)this)->thread;
    if(t != -1){
        ((preactor)this)->thread = -1;
        pthread_cancel(t); // ask the thread to stop
        pthread_join(t,NULL); // wait until the thread will stop
    }
}

void startReactor(void* this){

}







