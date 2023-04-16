#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy(char* file_name1, char* file_name2, int flagF){
        FILE *file1, *file2;

    file1 = fopen(file_name1, "rb");
    if(file1 == NULL){
        perror("error while opening file");
        exit(1);
    }

    if(!flagF){
        file2 = fopen(file_name2, "wb");
        if(file1 == NULL){
            perror("error while opening file");
            exit(1);
        }
    }

    else{
        
    }
}