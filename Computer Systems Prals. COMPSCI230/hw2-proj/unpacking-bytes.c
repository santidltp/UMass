/***************************************************************
 * This class has been developed with main pupose of unpucking *
 * chunks of 8bits to assign them to a vatiable that later on  *
 * is goint to be printed out as its ASCII value.              *
 * Develeped by Santiago De La Torre Pinzon                    *
 * at the University of Massachusetts at Amherst.              *
 **************************************************************/
#include <stdio.h>//for printf function
int main(){

unsigned int i1 = 1835098984u;//given 
unsigned int i2 = 1768842611u;//given
/*****************************************************************
 * These are the new variables that are going to be assigned     *
 * with the 8bits of representation from the numbers above.      *
 * We have 8 variables of char type, because char holds up a     *  
 * maximum of 8bits or 1byte and unsigened integer can hold      *
 * up to 32bits or 4bytes. Therefore, one variable of unsigned   *
 * integer is equivalent to 4 char variables, each one with.     *
 * 8 bits that would have an ASCII representation.               *
 *****************************************************************/
char c1,c2,c3,c4,c5,c6,c7,c8;
c1= (i1>>24) ; //m
c2= (i1>>16);  //a
c3= i1>>8;     //c
c4= i1>>0;     //h
c5= (i2>>24);  //i
c6= (i2>>16);  //n
c7= i2>>8;     //e
c8= i2>>0;     //s

printf("%c%c%c%c%c%c%c%c\n",c1,c2,c3,c4,c5,c6,c7,c8);
}