/**************************************************************
 * This class has been developed with main pupose of printing *
 * floats with decimals and their size.                       *
 * Develeped by Santiago De La Torre Pinzon                   *
 * at the University of Massachusetts at Amherst.             *
 **************************************************************/

#include <stdio.h>//added for the printf funciton.
int main(){

	float x=10;
	float y=13;
	printf("x = %0.6f\n", x);
	printf("y = %0.6f\n", y);
	printf("size of signed float in bytes is %d.\n", sizeof(x));
	printf("size of signed float in bits is %d.\n", sizeof(x)*8);//1byte=8bits
	printf("%0.6f + %0.6f = %0.6f.\n",x,y,x+y);
	printf("%0.6f + %0.6f = %d.\n",x,y,(int)(x+y));
}