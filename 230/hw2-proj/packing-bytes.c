/**************************************************************
 * This class has been developed with main pupose of packing  *
 * chunks of 8bits to a single unsgigned int variable.        *
 * Develeped by Santiago De La Torre Pinzon                   *
 * at the University of Massachusetts at Amherst.             *
 **************************************************************/
#include <stdio.h>//Included for the printf function
int main(){				   //using ASCII
	unsigned char b3 = 202;//CA
	unsigned char b2 = 254;//FE
	unsigned char b1 = 186;//BA
	unsigned char b0 = 190;//BE
	unsigned int u=0; 
	
	u = (b3<<24) | (b2<<16) | (b1<<8)| (b0<<0);
	    //print the packed int in hexadecimal CAFEBABE     
		printf("%X\n",u);
}