
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
#define DBG(fmt, args...) printf("%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, args)
#else
#define DBG(fmt, args...) 
#endif

int lengthOfInt(int num){
	int length = 0;
	while(num!=0){
		num = num/10;
		length++;
	}
	return length;
}
/*

1234
x_plus  = 34
x_moins = 12

*/
int multiply(int x, int y){
	if(x<10) return x*y;
	int n = lengthOfInt(x);

	int tmpx = x;
	int tmpy = y;

	int x_plus, x_less, y_plus, y_less;
	x_less = x_plus = y_less = y_plus = 0;

	for(int i=0; i<n/2; ++i){
		x_less = x_less*10 + tmpx%10;
		y_less = y_less*10 + tmpy%10;
		tmpy = tmpy/10;
		tmpx = tmpx/10;
		printf("x = %d, y = %d\n", x_less, y_less);
		printf("tmpx = %d\n\n", tmpx);
	}

	tmpx = x;
	tmpy = y;

	for(int i=0; i<n/2; ++i){
		x_plus = x_plus*10 + tmpx%10;
		y_plus = y_plus*10 + tmpy%10;
		tmpy = tmpy/10;
		tmpx = tmpx/10;
	}

	int p1 = multiply(x_plus, y_plus);
	int p2 = multiply(x_plus, y_less);
	int p3 = multiply(x_less, y_plus);
	int p4 = multiply(x_less, y_less);
	int a = p2 + p3;

	return p1*pow(10,n)+a*pow(10,n/2)+p4;
}
int main(void){
    int x = 1234;
    int y = 9876;

    
    /*if (getn(x) != getn(y)) {
            printf("Number of digits for x and y should be the same! \n");
            exit(1);
    }*/

    printf("%d * %d = %d \n You should get 12186984 !", x, y, multiply(x, y));
    return 0;
}