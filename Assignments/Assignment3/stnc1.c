#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
#include<string.h>

int portHandler(int port){
    if((port < 1024) || (port > 65535)){
        printf("PORT should be numerical between 1024 and 65535 included\n");
        exit(1);
    }
    return 0;
}

int IPHandler(char ip[20]){
    char* token = strtok(ip, ".");
    int dot_count = 0;
    while (token != NULL) {
        int num = atoi(token);
        if (num < 0 || num > 255) {
            printf("Invalid IP address\n");
            exit(1);
        }
        token = strtok(NULL, ".");
        dot_count++;
    }

    if (dot_count != 4) {
        printf("Invalid IP address\n");
        exit(1);
    }

    printf("Valid IP address\n");
    return 0;

}

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("client side usage: ./stnc -c IP PORT\n");
        printf("server side usage: ./stnc -s PORT\n");
        exit(1);
    }
    
    if(!(strcmp(argv[1], "-c"))){
        if(argc != 4){
            printf("client side usage: ./stnc -c IP PORT\n");
            printf("server side usage: ./stnc -s PORT\n");
            exit(1);
        }
        int PORT = atoi(argv[3]);
        char IP[20] = argv[2];
        portHandler(PORT);
        IPHandler(IP);

        //initializing a TCP socket.
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1) {
            printf("Could not create socket.\n");
            exit(1);
        }

        struct sockaddr_in receiver_adderess;
        //setting to zero the struct senderAddress
        memset(&receiver_adderess,0,sizeof(receiver_adderess));
        receiver_adderess.sin_family = AF_INET;
        receiver_adderess.sin_port = htons(PORT);
        int checkP = inet_pton(AF_INET,(const char*)IP,&receiver_adderess.sin_addr);
        
        if(checkP < 0){
            printf("inet_pton() FAILED.\n");
            exit(1);
        }
        
        //connecting to the Receiver on the socket
        int connectCheck = connect(sock,(struct sockaddr*) &receiver_adderess,sizeof(receiver_adderess));
        
        if(connectCheck == -1){
            printf("connect() FAILED.\n");
            exit(1);
        }
    }
    else if(!(strcmp(argv[1], "-s"))){
        if(argc != 3){
            printf("client side usage: ./stnc -c IP PORT\n");
            printf("server side usage: ./stnc -s PORT\n");
            exit(1);
        }
        int PORT =  atoi(argv[2]);
        char IP[] = "0.0.0.0";
        portHandler(PORT);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in senderAddress;

        //setting to zero the struct senderAddress
        memset(&senderAddress, 0, sizeof(senderAddress));
        senderAddress.sin_family = AF_INET;
        senderAddress.sin_port = htons(PORT);
        int checkP = inet_pton(AF_INET, (const char *) IP, &senderAddress.sin_addr);
        if (checkP <= 0) {
            printf("inet_pton() failed.\n");
            exit(1);
        }

        //opening the socket.
        int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
        if (Bcheck == -1) {
            printf("Error while binding.\n");
            exit(1);
        }

        //start listening on the socket (one client at the time)
        int Lcheck = listen(sock, 1);
        if (Lcheck == -1) {
            printf("Error in listen().\n");
            exit(1);
        }

        //accepting the client (the Sender)
        unsigned int senderAddressLen = sizeof(senderAddress);
        int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
        if (senderSock == -1) {
            printf("accept() failed.\n");
            close(sock);
            exit(1);
        }
    }
    else{
        printf("client side usage: ./stnc -c IP PORT\n");
        printf("server side usage: ./stnc -s PORT\n");
        exit(1);
    }
}
