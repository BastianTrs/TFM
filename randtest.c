#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){

	int i =0;
	double num = 0.0;
	srand48(time(NULL));
	for(i=0;i<10;i++){
		num = drand48()-0.5;
		printf("%f\n",num);
	}

	return 0;	
}
