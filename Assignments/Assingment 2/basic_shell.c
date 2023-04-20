#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;
    printf("%d\n", getpid());

    // ignoring ctrl + c
    signal(SIGINT, SIG_IGN);

    while (1) {
//        printf("aviv&alon$ ");
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0


        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL) {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;
        int argc = i;
        if (argc > 0) {
            if (strcmp("exit", argv[0]) == 0) {
                exit(1);
            }
        }

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        /* for commands not part of the shell command language */
        int id = fork();
        if (id == 0) {
            signal(SIGINT, SIG_DFL);

            int is_simple = 1;
            int pipes_n = 0;
            int pipes[2] = {-1, -1};
            int error = 0;
            int redirect = 0;
            int redirect_to = 0;

            for (int j = 1; j < argc; ++j) {
                if (strcmp(argv[j], ">") == 0) {
                    redirect = 1;
                    redirect_to = j + 1;
                    is_simple = 0;
                } else if (strcmp(argv[j], ">>") == 0) {
                    redirect = 2;
                    redirect_to = j + 1;
                    is_simple = 0;
                } else if (strcmp(argv[j], "|")) {
                    pipes[pipes_n++] = j;
                    is_simple = 0;
                }
            }

            if (redirect != 0) {
                if ( argc -1 <= redirect_to) {
                    printf("You should indicate the file name after the redirect (>)\n");
                    error = 1;
                } else {
                    FILE *fd2 = fopen(argv[redirect_to], "w");
                    int fout2 = fileno(fd2);
                    dup2(1, fout2);
                    fclose(fd2);
                }
            } else if (strcmp(argv[redirect_to], ">>")) {
                if (argc -1 <= redirect_to) {
                    printf("You should indicate the file name after the redirect (>>)\n");
                    error = 1;
                } else {
                    FILE *fd2 = fopen(argv[redirect_to], "a");
                    int fout2 = fileno(fd2);
                    dup2(1, fout2);
                    fclose(fd2);
                }

            }
            if (!error) {
                if (is_simple)
                    execvp(argv[0], argv);
                else {

                }

                wait(NULL);
            }
        }
    }
}
