#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main() {
    int status;
    int argc;
    char *argv[20];
    char command[1024];
    char *token;

    // ignoring ctrl + c
    signal(SIGINT, SIG_IGN);

    while (1) {
        char path[1024];
        getcwd(path, sizeof(path));
        printf("\033[1;32mAviv&Alon's_shell\033[0m:\033[1;34m~%s\033[37m$\033[0m ", path);
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        argc = 0;
        token = strtok(command, " ");
        while (token != NULL) {
            argv[argc] = token;
            token = strtok(NULL, " ");
            argc++;
        }
        argv[argc] = NULL;
        if (argc > 0) {
            if (strcmp("exit", argv[0]) == 0) {
                exit(0);
            }
        }

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        int is_simple = 1;
        int pipes_n = 0;
        int pipes[2] = {-1, -1};
        int redirect = 0;
        int redirect_to = 0;

        for (int j = 1; j < argc; ++j) {
            if (strcmp(argv[j], ">") == 0) {
                redirect = 1;
                redirect_to = j + 1;
                is_simple = 0;
                // printf("found >\n");
            } else if (strcmp(argv[j], ">>") == 0) {
                redirect = 2;
                redirect_to = j + 1;
                is_simple = 0;
                // printf("found >>\n");
            } else if (strcmp(argv[j], "|") == 0) {
                // printf("found | in %d\n ", j);
                pipes[pipes_n++] = j;
                is_simple = 0;
            }
        }

        /* for commands not part of the shell command language */
        pid_t id1 = fork();
        if (id1 != 0) {
            wait(&status);
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code) {
                    printf("\033[0;31merror: \033[0m");
                    if(exit_code == 1){
                        printf("Failed while openning pipes\n");
                    }
                    if(exit_code == 2){
                        printf("Missing file name after `>`\n");
                    }
                    if(exit_code == 3){
                        printf("Missing file name after `>>`\n");
                    }
                    printf("(Exit code %d)\n", exit_code);
                }
            }
        }
        else {
            signal(SIGINT, SIG_DFL);

            if (is_simple) {
                execvp(argv[0], argv);
            } else if (pipes_n == 1) {
                int fd[2];
                if (pipe(fd) == -1) {
                    exit(1);
                }

                pid_t id2 = fork();
                if (id2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], 1); 
                    close(fd[1]);
                    char *argv2[10];
                    int j;
                    for (j = 0; j < pipes[0]; ++j) {
                        argv2[j] = argv[j];
                    }
                    argv2[j] = NULL;
                    execvp(argv2[0], argv2);
                } else {
                    pid_t id3 = fork();
                    if (id3 == 0) {
                        waitpid(id2, NULL, 0);
                        close(fd[1]);
                        dup2(fd[0], 0);
                        close(fd[0]);

                        int till = argc;
                        if (redirect) {
                            till = redirect_to - 1;
                            if(redirect == 1){
                                if (argc <= redirect_to) {
                                    printf("\033[0;31merror: \033[0m");
                                    printf("Missing file name after `>`\n");
                                    printf("(Exit code 2)\n");
                                    exit(2);
                                } else {
                                    FILE *fd = fopen(argv[redirect_to], "w");
                                    int fout = fileno(fd);
                                    dup2(fout, 1);
                                    fclose(fd);
                                }
                            }
                            if(redirect == 2){
                                if ( argc <= redirect_to) {
                                    printf("\033[0;31merror: \033[0m");
                                    printf("Missing file name after `>>`\n");
                                    printf("(Exit code 3)\n");
                                    exit(3);
                                } else {
                                    FILE *fd = fopen(argv[redirect_to], "a");
                                    int fout = fileno(fd);
                                    dup2(fout, 1);
                                    fclose(fd);
                                }
                            }
                        }

                        char *argv3[10];
                        int i, k;
                        for (i = pipes[0] + 1, k = 0; i < till; ++i, k++) {
                            argv3[k] = argv[i];
                        }
                        argv3[k] = NULL;
                        execvp(argv3[0], argv3);
                    } else {
                        close(fd[0]);
                        close(fd[1]);
                        waitpid(id3, NULL, 0);
                    }
                }
            } else if (pipes_n == 2) {
                int fd1[2], fd2[2];
                if (pipe(fd1) == -1 || pipe(fd2) == -1) {
                    exit(1);
                }

                pid_t id2 = fork();
                if (id2 == 0) {
                    close(fd2[0]);
                    close(fd2[1]);

                    close(fd1[0]);
                    dup2(fd1[1], 1);
                    close(fd1[1]);

                    char *argv2[10];
                    int j, k;
                    for (j = 0, k = 0; j < pipes[0]; ++j, ++k) {
                        argv2[k] = argv[j];
                    }
                    argv2[k] = NULL;
                    execvp(argv2[0], argv2);
                } else {
                    pid_t id3 = fork();
                    if (id3 == 0) {
                        waitpid(id2, NULL, 0);
                        close(fd1[1]);
                        dup2(fd1[0], 0);
                        close(fd1[0]);

                        close(fd2[0]);
                        dup2(fd2[1], 1);
                        close(fd2[1]);

                        int till = pipes[1];
                        char *argv3[10];
                        int i, k;
                        for (i = pipes[0] + 1, k = 0; i < till; ++i, ++k) {
                            argv3[k] = argv[i];
                        }
                        argv3[k] = NULL;
                        execvp(argv3[0], argv3);
                    } else {
                        pid_t id4 = fork();
                        if (id4 == 0) {
                            waitpid(id3, NULL, 0);
                            close(fd1[0]);
                            close(fd1[1]);

                            close(fd2[1]);
                            dup2(fd2[0], 0);
                            close(fd2[0]);

                            int till = argc;
                            if (redirect) {
                                till = redirect_to - 1;
                                if(redirect == 1){
                                    if ( argc <= redirect_to) {
                                        printf("\033[0;31merror: \033[0m");
                                        printf("Missing file name after `>`\n");
                                        printf("(Exit code 2)\n");
                                        exit(2);
                                    } else {
                                        FILE *fd = fopen(argv[redirect_to], "w");
                                        int fout = fileno(fd);
                                        dup2(fout, 1);
                                        fclose(fd);
                                    }
                                }
                                if(redirect == 2){
                                    if ( argc <= redirect_to) {
                                        printf("\033[0;31merror: \033[0m");
                                        printf("Missing file name after `>>`\n");
                                        printf("(Exit code 3)\n");
                                        exit(3);
                                    } else {
                                        FILE *fd = fopen(argv[redirect_to], "a");
                                        int fout = fileno(fd);
                                        dup2(fout, 1);
                                        fclose(fd);
                                    }
                                }
                            }

                            char *argv4[10];
                            int i, k;
                            for (i = pipes[1] + 1, k = 0; i < till; ++i, ++k) {
                                argv4[k] = argv[i];
                            }
                            argv4[k] = NULL;
                            execvp(argv4[0], argv4);
                        } else {
                            close(fd1[0]);
                            close(fd1[1]);
                            close(fd2[1]);
                            close(fd2[0]);
                            waitpid(id4, NULL, 0);
                        }
                    }
                }
            } else if (redirect == 1) {
                if ( argc <= redirect_to) {
                    exit(2);
                } else {
                    char *argv2[10];
                    int i;
                    for(i = 0; i < redirect_to - 1; i++){
                        argv2[i] = argv[i];
                    }
                    argv2[i] = NULL;
                    FILE *fd = fopen(argv[redirect_to], "w");
                    int fout = fileno(fd);
                    dup2(fout, 1);
                    fclose(fd);
                    execvp(argv2[0], argv2);
                }
            } else if (redirect == 2) {
                if ( argc <= redirect_to) {
                    exit(3);
                } else {
                    char *argv2[10];
                    int i;
                    for(i = 0; i < redirect_to - 1; i++){
                        argv2[i] = argv[i];
                    }
                    argv2[i] = NULL;
                    FILE *fd = fopen(argv[redirect_to], "a");
                    int fout = fileno(fd);
                    dup2(fout, 1);
                    fclose(fd);
                    execvp(argv2[0], argv2);
                }
            }
            wait(NULL);
            exit(0);
        }
    }
}