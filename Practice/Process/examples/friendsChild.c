#include <unistd.h>
#include <stdio.h>


void print(const char *text){

	int i;
	for (i=0;i<20;i++){
		printf("Hello from %s\n",text);
		usleep(1000000);
	}

}

int main()
{

	print("friends child");

	return 0;
}
