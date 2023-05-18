//
// Created by alon on 5/18/23.
//

#include "st_reactor.h"
#include <poll.h>

void *thread_func(void *phash);

void *createReactor() {
    preactor new_reactor = malloc(sizeof(reactor));
    new_reactor->thread = -1;

    new_reactor->WaitFor = WaitFor;
    new_reactor->addFd = addFd;
    new_reactor->startReactor = startReactor;
    new_reactor->stopReactor = stopReactor;

    PhashMap hashMap = malloc(sizeof(hashMap));
    initializeHashMap(hashMap);
    new_reactor->hash = hashMap;

    return new_reactor;
}

void stopReactor(void *this) {
    int t = ((preactor) this)->thread;
    if (t != -1) {
        ((preactor) this)->thread = -1;
        pthread_cancel(t); // ask the thread to stop
        pthread_join(t, NULL); // wait until the thread will stop
    }
}

void startReactor(void *this) {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, ((preactor) this)->hash);
    ((preactor) this)->thread = thread;

}

void cleanupHandler(void *arg) { //from gpt,  asked it: i want to clean up allocated space before canceling my thread.
    // Cleanup code here
    free(arg);
}

void *thread_func(void *phash_param) {
    PhashMap phash = phash_param;
    int my_fds_num = 0;
    struct pollfd *fds = malloc(sizeof(struct pollfd) * (phash->numOfElements)); // todo: free
//    struct pollfd fds2[phash->numOfElements];
    int index = 0;
    // fill the fds array
    for (int i = 0; i < phash->capacity; ++i) {
        if (phash->arr[i] != NULL) {
            pnode temp = phash->arr[i];
            do {
                fds[index].fd = temp->key;
                fds[index].events = POLLIN;
                temp = temp->next;
                index++;
            } while (temp != NULL);
        }
    }
    my_fds_num = index;
    if (index != phash->numOfElements) {
        printf("missed some fds ?\n");
        printf("expected: %d, found %d\n", phash->numOfElements, my_fds_num);
    }


    pthread_cleanup_push(cleanupHandler, fds);

            while (1) {
                // check for cancel
                pthread_testcancel();
                //pool handle
                int err = poll(fds,my_fds_num,1000);
                if (err < 0) {
                    printf("poll failed\n");
                }
                for (int i = 0; i < my_fds_num; ++i) {
                    if(fds[i].revents && POLLIN){
                        void* func = search(phash,fds[i].fd);
                        // todo: convert func to the right function (somehow) and activate it.
                    }
                }
                if(my_fds_num < phash->numOfElements){
                    //todo: update the fds list
                }

            }
    pthread_cleanup_pop(1);

}


