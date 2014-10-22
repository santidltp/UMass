/***************************************************************
 * This class has been developed with main purpose of printing *
 * bit character.                                              *
 * Developed by Santiago De La Torre Pinzon                    *
 * at the University of Massachusetts at Amherst.              *
 ***************************************************************/

//Necessary for pinting on the screen, using printf.
#include <stdio.h>//print

//Function prototype
void print_bits(char c); 


int main(){
unsigned char c1 =181;
signed char c2 = -75;

//Function calls
print_bits(c1);
print_bits(c2);

}
//Function definition
void print_bits(char c){
int x;
/*************************************************************
 * We have to decrement our for-loop since we are printing   *
 * from left to right. We read our variable bit by bit. If is*
 * 0, then we print out 0, otherwise we print 1.             *
 *************************************************************/
	for( x=7; x>=0; x--){
	print
		if((c &(1<<x)) == 0) 
			printf("0");
		else 
			printf("1");
	}
printf("\n");
}