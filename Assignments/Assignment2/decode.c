#include <stdio.h>
#include <dlfcn.h>
#include "string.h"
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc < 2){
        printf("please use the format: ./encode <codec> <message>\n");
        return 1;
    }

    int total_length = 0;
    for (int i = 1; i < argc; i++) {
        total_length += strlen(argv[i]) + 1;
    }

    char* message = (char*) malloc(total_length);
    if (!message) {
        printf("malloc failed");
        return 1;
    }
    message[0] = '\0';


    for (int i = 2; i < argc ; ++i) {
        strcat(message,argv[i]);
        if(i+1 < argc)
            strcat(message," ");

    }
    strcat(message,"\0");

    char * codec = argv[1];
    char* library_name = (char*)malloc(strlen(codec) + 10);
    sprintf(library_name, "./%s", codec);
    void* program = dlopen(library_name, RTLD_NOW);
    free(library_name);

    if(!program){
        printf( "Error: %s\n", dlerror());
        printf("error (1)\n");
        return 1;
    }
    void (*decode)(char*) = dlsym(program,"decode");
    if(!decode){
        printf("error (2)\n");
        return 1;
    }
    decode(message);
    printf("decode %s %s\n",argv[1],message);
    dlclose(program);
    free(message);
    return 0;
}