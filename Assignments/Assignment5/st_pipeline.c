#include "st_pipeline.h"

int generateRandomNumber() {

    int randomNum = 0;
    int min = 100000;  // Minimum value for a 6-digit number
    int max = 999999;  // Maximum value for a 6-digit number

    // Generate random number within the desired range
    randomNum = (rand() % (max - min + 1)) + min;

    return randomNum;
}

void func4(int num, pparam p) {
    printf("%d\n", num);
    num += 2;
    printf("%d\n", num);
    *(p->flag) = *(p->flag) - 1;
}

void func3(int num, pparam p) {
    printf("%d\n", num);
    if (num < 0) // not necessary
        num = -num;
    unsigned int u_num = (unsigned int) num;
    if (isPrime(u_num)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    num = num - 13;

    p->next->queue->enqueue(p->next->queue, &num);


}

void func2(int num, pparam p) {
    printf("%d\n", num);
    if (num < 0) // not necessary
        num = -num;
    unsigned int u_num = (unsigned int) num;
    if (isPrime(u_num)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    num = num + 11;

    p->next->queue->enqueue(p->next->queue, &num);

}

void func1(int num, pparam p) {
    srand(p->seed);  // Set the seed for random number generation
    Queue *next_q = p->next->getQueue(p->next);
    for (int i = 0; i < p->N; ++i) {
        int rand = generateRandomNumber();
        next_q->enqueue(next_q, (void *) &rand);
        usleep(1000);
    }
}

int main(int argc, char *argv[]) {
    int seed = 0;
    if (argc != 2 && argc != 3) {
        return 1;
    } else if (argc == 2) {
        seed = time(NULL);
    } else {
        seed = atoi(argv[2]);
    }
    int N = atoi(argv[1]);


    int left = N;
    // create th active objects
    pparam p4 = malloc(sizeof(param));
    p4->this = NULL;
    p4->flag = &left;
    p4->next = NULL;
    p4->seed = seed;
    p4->N = N;
    AO *ao4 = CreateActiveObject(func4, p4);

//    param p3 = {NULL, ao4, N, seed, &left};
    pparam p3 = malloc(sizeof(param));
    p3->this = NULL;
    p3->flag = &left;
    p3->next = ao4;
    p3->seed = seed;
    p3->N = N;
    AO *ao3 = CreateActiveObject(func3, p3);

    pparam p2 = malloc(sizeof(param));
    p2->this = NULL;
    p2->flag = &left;
    p2->next = ao3;
    p2->seed = seed;
    p2->N = N;
    AO *ao2 = CreateActiveObject(func2, p2);

    pparam p1 = malloc(sizeof(param));
    p1->this = NULL;
    p1->flag = &left;
    p1->next = ao2;
    p1->seed = seed;
    p1->N = N;
    AO *ao1 = CreateActiveObject(func1, p1);
    usleep(1000000);
    ao1->getQueue(ao1)->enqueue(ao1->getQueue(ao1), &left); //enqueue some data so the first AO will start.

    while (left > 0) {
        usleep(1000);
    }
    // kill threads
    ao1->stop(*ao1);
    ao2->stop(*ao2);
    ao3->stop(*ao3);
    ao4->stop(*ao4);
    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return 0;
}






