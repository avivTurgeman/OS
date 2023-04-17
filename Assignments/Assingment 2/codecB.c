void encode(char* str, int size){
    for(int i = 0; i < size; i++){
        str[i] += 3;
    }
}

void decode(char* str, int size){
    for(int i = 0; i < size; i++){
        str[i] -= 3;
    }
}
