#include <stdio.h>
#include <dlfcn.h>
#include "string.h"

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("please use the format: ./encode <codec> <message>\n");
        return 1;
    }
    char * codec = argv[1];
    char* message = argv[2];
    for (int i = 3; i < argc ; ++i) {
        strcat(message,argv[i]);
    }
    strcat(message,"\0");
    void * program = dlopen(argv[1],RTLD_NOW);
    printf("%s\n",argv[1]);
    if(!program){
        fprintf(stdout, "Error: %s\n", dlerror());
        printf("error (1)\n");
        return 1;
    }
    void (*encode)(char*) = dlsym(program,"encode");
    if(!encode){
        printf("error (2)\n");
        return 1;
    }
    printf("encode %p %s",codec,message);
    dlclose(program);
    return 0;
}