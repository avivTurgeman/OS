#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void print(const char *text){

	int i;
	for (i=0;i<20;i++){
		printf("Hello from %s\n",text);
		usleep(1000000);
	}
}

int main()
{

	printf("pid: %ld  , ppid %ld \n", (long)getpid(), (long)getppid());

	//create child
	pid_t pid = fork();

	if (pid == 0){
		//we are in child

		// create a grand child
		pid_t pid = fork();

		if (pid == 0){
			//we are in grand child
			//print("grand child");

			char * args[2] = {"./friend",NULL}; 
			execvp(args[0],args); 
		}
		else{
			print("child");
		}
	}
	else{
		print("parent");
	}

        return 0;
}


