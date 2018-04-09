
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

void resize(int* T, int n, int old_size){

	int *P = malloc(n*sizeof(int));
	for(int i=0 ; i<n-old_size; ++i) P[i] = 0;
	for(int i=old_size; i<n; ++i) P[i] = T[i];
	T = realloc(T, sizeof(int) * (n));
	for(int i=0; i<n; ++i) T[i] = P[i];
}

int multiply(int x, int y){
	if(x<10) return x*y;
int x_len = lengthOfInt(x);
int y_len = lengthOfInt(y);

int* X = malloc(x_len*sizeof(int));
int* Y = malloc(y_len*sizeof(int));
intToArray(x, X);
intToArray(y, Y);

if(x_len > y_len) resize(Y, x_len, y_len);
if(y_len > x_len) resize(X, y_len, x_len);


	return 1;
}


int main(void){
    //int x = 1234;
    //int y = 9876;
		int *X = malloc(3*sizeof(int));
		X[0] = X[1] = X[2] = 8;
		resize(X, 5, 3);
		for(int i=0; i<5; ++i) printf("%d,", X[i]);
		printf("\n");

    //printf("\n%d * %d =  %d \nYou should get 12186984 !\n", x, y, multiply(x, y));
    return 0;
}
