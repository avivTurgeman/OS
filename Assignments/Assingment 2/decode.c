#include <stdio.h>
#include <dlfcn.h>
#include "string.h"

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("please use the format: ./decode <codec> <message>\n");
        return 1;
    }
    char * codec = argv[1];
    char* message = argv[2];
    for (int i = 3; i < argc ; ++i) {
        strcat(message,argv[i]);
    }
    strcat(message,"\0");
    void * program = dlopen(codec,RTLD_LAZY);
    if(!program){
        printf("error (1)\n");
    }
    void (*encode)(char*) = dlsym(program,"decode");
    if(!encode){
        printf("error (2)\n");
        return 1;
    }
    printf("decode %p %s",codec,message);
    dlclose(program);
    return 0;
}