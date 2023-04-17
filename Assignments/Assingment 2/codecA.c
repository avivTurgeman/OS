#define DIFF 'A' - 'a'

void encode(char* str){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            str[i] += DIFF;
        }
        else if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] -= DIFF;
        }
    }
}

void decode(char* str){
    encode(str);
}