#define DIFF 'A' - 'a'

void encode(char* str, int size){
    for(int i = 0; i < size; i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            str[i] += DIFF;
        }
        else if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] -= DIFF;
        }
    }
}

void decode(char* str, int size){
    encode(str, size);
}