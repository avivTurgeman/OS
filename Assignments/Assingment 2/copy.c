#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int copy(char* file_name1, char* file_name2, int flagF){
    FILE *file1, *file2;

    file1 = fopen(file_name1, "rb");
    if(file1 == NULL){
        perror("error while opening file");
        return -1;
    }

    if(flagF || (access(file_name2, F_OK))){
        file2 = fopen(file_name2, "wb");
        if(file2 == NULL){
            perror("error while opening file");
            return -1;
        }
        size_t bytes_read;
        char buffer[1];
        while ((bytes_read = fread(buffer, sizeof(char), 1, file1)) == 1){
            fwrite(buffer, sizeof(char), 1, file2);
        }
        fclose(file1);
        fclose(file2);
        return 0;
    }

    else{
        fclose(file1);
        return 1;
    }
}
   
int main(int argc, char *argv[]){
    char* file_name1 = argv[1];
    char* file_name2 = argv[2];
    int flagF = 0, flagV = 0;
    int rv;

    for(int i = 3; i < argc; i++){
        if(!strcmp(argv[i], "-v")){
            flagV = 1;
        }
        if(!strcmp(argv[i], "-f")){
            flagF = 1;
        }
    }

    rv = copy(file_name1, file_name2,flagF);

    
    if(flagV){
        if(rv == -1){
            printf("general failure\n");
            return 1;
        }
        else if(rv == 0){
            printf("success\n");
        }
        else{
            printf("target file exist\n");
        }
    }
    return rv;
}