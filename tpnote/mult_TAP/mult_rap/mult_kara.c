
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int lengthOfInt(int num){
	int length = 0;
	while(num!=0){
		num = num/10;
		length++;
	}
	return length;
}

void intToArray(int n, int* t){
	int lenght = lengthOfInt(n);

	for(int i=lenght-1; i>=0; --i){
		t[i] = n%10;
		n = n/10;
	}
}

int multiply(int x, int y){
	return 1;
}


int main(void){
    //int x = 1234;
    //int y = 9876;

    //printf("\n%d * %d =  %d \nYou should get 12186984 !\n", x, y, multiply(x, y));
    return 0;
}
