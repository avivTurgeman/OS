//
// Created by alon on 5/31/23.
//

#include "st_pipeline.h"

//from gpt. request: i want to generate randome number with a length of 6 digits in
//                   a function, that function gets a seed number to generate
int generateRandomNumber() {

    int randomNum = 0;
    int numDigits = 6;
    int min = 100000;  // Minimum value for a 6-digit number
    int max = 999999;  // Maximum value for a 6-digit number

    // Generate random number within the desired range
    randomNum = (rand() % (max - min + 1)) + min;

    return randomNum;
}

void func4(int num, pparam p){
    printf("ao 4: "); // debug
    printf("%d\n",num);
    num += 2;
    printf("%d\n",num);
    *(p->flag)--;
}

void func3(int num, pparam p){
    printf("ao 3: "); // debug
    printf("%d\n",num);
    if(num <0) // not necessary
        num = -num;
    unsigned int u_num = (unsigned int) num;
    if(isPrime(u_num)){
        printf("true\n");
    } else{
        printf("false\n");
    }
    num = num - 13;

    p->next->queue->enqueue(p->next->queue,&num);


}

void func2(int num, pparam p){
    printf("ao 2: "); // debug
    printf("%d\n",num);
    if(num <0) // not necessary
        num = -num;
    unsigned int u_num = (unsigned int) num;
    if(isPrime(u_num)){
        printf("true\n");
    } else{
        printf("false\n");
    }
    num = num + 11;

    p->next->queue->enqueue(p->next->queue,&num);

}

void func1(int num, pparam p) {
    srand(p->seed);  // Set the seed for random number generation
    Queue * next_q = p->next->getQueue(*p->next);
    for (int i = 0; i < p->N; ++i) {
        int rand = generateRandomNumber();
        next_q->enqueue(next_q,(void *)&rand);
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
    param p4 = {NULL, NULL, N, seed, &left};
    AO *ao4 = CreateActiveObject(func4, &p4);

    param p3 = {NULL, ao4, N, seed, &left};
    AO *ao3 = CreateActiveObject(func3, &p3);

    param p2 = {NULL, ao3, N, seed, &left};
    AO *ao2 = CreateActiveObject(func2, &p2);

    param p1 = {NULL, ao2, N, seed, &left};
    AO *ao1 = CreateActiveObject(func1, &p1);

    ao1->getQueue(*ao1)->enqueue(ao1->getQueue(*ao1),&left); //enqueue some data so the first AO will start.

    while (left > 0){
        usleep(1000);
    }
    // kill threads
    ao1->stop(*ao1);
    ao2->stop(*ao2);
    ao3->stop(*ao3);
    ao4->stop(*ao4);

    return 0;
}






