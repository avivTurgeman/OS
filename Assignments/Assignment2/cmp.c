#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(char *file_name1, char *file_name2, int flagI){
    FILE *file1, *file2;

    file1 = fopen(file_name1, "rb");
    if(file1 == NULL){
        perror("file does not exist");
        exit(1);
    }

    file2 = fopen(file_name2, "rb");
    if(file1 == NULL){
        perror("file does not exist");
        exit(1);
    }

    while(!feof(file1) || !feof(file2)){
        char b1, b2;
        fread(&b1, sizeof(char), 1, file1);
        fread(&b2, sizeof(char), 1, file2);
        if(flagI){
            if((b1 - b2 != 0) && (b1 - b2 != ('a' - 'A')) && (b1 - b2 != ('A' - 'a'))){
                fclose(file1);
                fclose(file2);
                return 1;
            }
        }
        else if(b1 != b2){
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }

    if(feof(file1) != feof(file2)){
        fclose(file1);
        fclose(file2);
        return 1;
    }
    fclose(file1);
    fclose(file2);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("to use the program please use this format: ./cmp <file1> <file2>\n");
        printf("add -i to ignore lower/upper case\n");
        printf("add -v for verbal output\n");
        return 1;
    }
    char* file_name1 = argv[1];
    char* file_name2 = argv[2];
    int flagI = 0, flagV = 0;
    int func_ret;

    for(int i = 3; i < argc; i++){
        if(!strcmp(argv[i], "-v")){
            flagV = 1;
        }
        if(!strcmp(argv[i], "-i")){
            flagI = 1;
        }
    }

    func_ret = cmp(file_name1, file_name2,flagI);
    
    if(flagV){
        if(!func_ret){
            printf("equal\n");
        }
        else{
            printf("distinct\n");
        }
    }
    return func_ret;
}