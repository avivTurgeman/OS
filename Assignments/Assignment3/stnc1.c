#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
#include<string.h>
#include <poll.h>
#include <sys/time.h>

int portHandler(int port) {
    if ((port < 1024) || (port > 65535)) {
        printf("PORT should be numerical between 1024 and 65535 included\n");
        exit(1);
    }
    return 0;
}

int IPHandler(char ip[20]) {
    char *token = strtok(ip, ".");
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

    return 0;
}

int tcp_client_conn(char * ip, char *port){
    int PORT = atoi(port);
    char IP[40];
    strcpy(IP, ip);
    portHandler(PORT);
    IPHandler(IP);

    //initializing a TCP socket.
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket.\n");
        exit(1);
    }

    struct sockaddr_in receiver_adderess;
    //setting to zero the struct senderAddress
    memset(&receiver_adderess, 0, sizeof(receiver_adderess));
    receiver_adderess.sin_family = AF_INET;
    receiver_adderess.sin_port = htons(PORT);
    int checkP = inet_pton(AF_INET, (const char *) IP, &receiver_adderess.sin_addr);

    if (checkP < 0) {
        printf("inet_pton() FAILED.\n");
        exit(1);
    }

    //connecting to the Receiver on the socket
    int connectCheck = connect(sock, (struct sockaddr *) &receiver_adderess, sizeof(receiver_adderess));

    if (connectCheck == -1) {
        printf("connect() FAILED.\n");
        exit(1);
    }
    return sock;
}

void usage() {
    printf("Usage options:\n");
    printf("client side usage: ./stnc -c IP PORT\n");
    printf("server side usage: ./stnc -s PORT\n");
}

int client_A(char *port, char *ip) {

    int sock = tcp_client_conn(ip,port);
    int fd = -1;
    struct pollfd fds[2];
    fds[0].fd = 0; // stdin
    fds[0].events = POLLIN; // tell me when I can read from it
    fds[1].fd = sock;
    fds[1].events = POLLIN;
    while (1) {
        int err = poll(fds, 2, -1);
        if (err < 0) {
            printf("poll failed\n");
            return 1;
        }
        if (fds[0].revents && POLLIN) {
            // read from keyboard and send to the server
            char buffer[10000] = {'\0'};
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // remove newline character from the end of the line
                buffer[strcspn(buffer, "\n")] = '\0';
            }
            int size = strlen(buffer);
            if (send(sock, buffer, size, 0) < 0) {
                perror("could not send the data\n");
                return 1;
            }
        }
        if (fds[1].revents && POLLIN) {
            // read from sock and print out
            char buffer2[10000] = {'\0'};
            if (recv(sock, buffer2, 10000, 0) < 0) {
                perror("could not receive data\n");
                return 1;
            }
            printf("%s\n", buffer2);
        }
    }
    return 0;
}

int server_A(char *port) {

    int PORT = atoi(port);
    char IP[] = "0.0.0.0";
    portHandler(PORT);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in senderAddress;
    //setting to zero the struct senderAddress
    memset(&senderAddress, 0, sizeof(senderAddress));
    senderAddress.sin_family = AF_INET;
    senderAddress.sin_port = htons(PORT);

    //opening the socket.
    int Bcheck = bind(sock, (struct sockaddr *) &senderAddress, sizeof(senderAddress));
    if (Bcheck == -1) {
        return 1;
    }
    //start listening on the socket (one client at the time)
    int Lcheck = listen(sock, 1);
    if (Lcheck == -1) {
        printf("Error in listen().\n");
        return 1;
    }

    //accepting the client (the Sender)
    unsigned int senderAddressLen = sizeof(senderAddress);
    int senderSock = accept(sock, (struct sockaddr *) &senderAddress, &senderAddressLen);
    if (senderSock == -1) {
        printf("accept() failed.\n");
        close(sock);
        return 1;
    }
    int fd = -1;
    struct pollfd fds[2];
    fds[0].fd = 0; // stdin
    fds[0].events = POLLIN; // tell me when I can read from it
    fds[1].fd = senderSock;
    fds[1].events = POLLIN;
    while (1) {
        int err = poll(fds, 2, -1);
        if (err < 0) {
            perror("poll failed\n");
            return 1;
        }
        if (fds[0].revents && POLLIN) {
            // read from keyboard and send to the server
            char buffer[10000] = {'\0'};

            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // remove newline character from the end of the line
                buffer[strcspn(buffer, "\n")] = '\0';
            }
            int size = strlen(buffer);
            if (send(senderSock, buffer, size, 0) < 0) {
                perror("could not send the data\n");
                close(senderSock);
                return 1;
            }
        }
        if (fds[1].revents && POLLIN) {
            // read from sock and print out
            char buffer2[10000] = {'\0'};
            if (recv(senderSock, buffer2, 10000, 0) < 0) {
                perror("could not receive data\n");
                close(senderSock);
                return 1;
            }
            printf("%s\n", buffer2);
        }
    }

    return 0;
}

int type_param(char* ip,char* port,char * type, char * param){
    int sock = tcp_client_conn(ip, port);
    char * temp = strcat(type,",");
    char *message= strcat(temp,param);
    printf("check1: %s",message);
    send(sock,message, strlen(message),0);
    // Set socket options to allow reuse of address
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    close(sock);
}

int client_TCP_B(char * ip, char* port,FILE * file){

    int sock = tcp_client_conn(ip, port);

    char buffer[16384];
    size_t bytes_read;
    // struct timeval start, end, diff;
    // gettimeofday(&start,NULL);
    while ((bytes_read = fread(buffer, sizeof(buffer),1, file)) > 0) {
        if (send(sock, buffer, bytes_read, 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
    }
    // gettimeofday(&end,NULL);

    // timersub(&end,&start,&diff);
    // int microsec = diff.tv_usec;
}

int main(int argc, char *argv[]) {
    int is_tcp = 0;
    int is_udp = 0;
    int is_ip = 0;
    int is_port = 0;
    int is_ipv4 = 0;
    int is_ipv6 = 0;
    int is_uds = 0;
    int is_dgram = 0;
    int is_stream = 0;
    int is_mmap = 0;
    int is_pipe = 0;
    int is_c = 0;
    int is_p = 0;
    int is_q = 0;
    int is_server = 0;
    char file_name[50] = {'\0'};
    char ip[40] = {'\0'};
    char port[10] = {'\0'};
    if (argc < 3) {
        usage();
        exit(1);
    }
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0) {
            is_c = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            is_server = 1;
        } else if (strcmp(argv[i], "-p") == 0) {
            is_p = 1;
        } else if (strcmp(argv[i], "-q") == 0) {
            is_q = 1;
        } else if (strcmp(argv[i], "pipe") == 0) {
            is_pipe = 1;
            if (i + 1 >= argc) {
                usage();
                exit(1);
            }
            strcpy(file_name, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "mmap") == 0) {
            is_mmap = 1;
            if (i + 1 >= argc) {
                usage();
                exit(1);
            }
            strcpy(file_name, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "uds") == 0) {
            is_uds = 1;
        } else if (strcmp(argv[i], "ipv6") == 0) {
            is_ipv6 = 1;
        } else if (strcmp(argv[i], "ipv4") == 0) {
            is_ipv4 = 1;
        } else if (strcmp(argv[i], "tcp") == 0) {
            is_tcp = 1;
        } else if (strcmp(argv[i], "udp") == 0) {
            is_udp = 1;
        } else if (strstr(argv[i], ".") != NULL) {
            is_ip = 1;
            strcpy(ip, argv[i]);
        } else if (strstr(argv[i], ":") != NULL) {
            is_ip = 1;
            strcpy(ip, argv[i]);
        } else if (strcmp(argv[i], "dgram") == 0) {
            is_dgram = 1;
        } else if (strcmp(argv[i], "stream") == 0) {
            is_stream = 1;
        } else {
            is_port = 1;
            strcpy(port, argv[i]);
        }
    }

    if (!is_p) { //part A
        if (is_c) { //client A
            if (argc != 4) {
                usage();
                exit(1);
            }
            if (!is_port || !is_ip) {
                usage();
                exit(1);
            }
            if (client_A(port, ip) != 0)
                exit(1);

        } else if (is_server) { // server A
            if (argc != 3) {
                usage();
                exit(1);
            }
            if (!is_port) {
                usage();
                exit(1);
            }
            if (server_A(port) != 0)
                exit(1);
        } else {
            usage();
            exit(1);
        }
    } else { // part B
        if (is_c) { // client side
            FILE *file;
            file = fopen("100MB.bin", "rb");
            if (file == NULL) {
                perror("File open failed");
                exit(EXIT_FAILURE);
            }
            //socket to notify the receiver to start timing
            int notify_socket = tcp_client_conn(ip, port+1);
            char notify[16] = "start,";

            if (is_ipv4 && is_tcp) { // ipv4 - tcp
                int sock = tcp_client_conn(ip,port);
                char buffer[16384];
                size_t bytes_read;
                while ((bytes_read = fread(buffer, sizeof(buffer),1, file)) > 0) {
                    if (send(sock, buffer, bytes_read, 0) == -1) {
                        perror("Send failed");
                        exit(EXIT_FAILURE);
                    }
                }

            } else if (is_ipv4 && is_udp) { // ipv4 - udp

            } else if (is_ipv6 && is_tcp) { // ipv6- tcp

            } else if (is_ipv6 && is_udp) { // ipv6 - udp

            } else if (is_uds && is_dgram) { // uds dgram

            } else if (is_uds && is_stream) {// uds stream

            } else if (is_mmap) { // mmap

            } else if (is_pipe) { // pipe

            } else {
                usage();
                exit(1);
            }
        } else if (is_server) {

        } else {
            usage();
            exit(1);
        }
    }
}