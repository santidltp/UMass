/**************************************************************
 * This class has been developed with main pupose of printing *
 * characters and their size.                                 *
 * Develeped by Santiago De La Torre Pinzon                   *
 * at the University of Massachusetts at Amherst.             *
 **************************************************************/
#include <stdio.h>//needed for the printf funcion.

int main()
{
	char c = 'C';
	char a = 65;
	char b = 'B';
	char f= 'F';
	char e= 'E';


	printf("c = %c\n", c);
	printf("a = %c\n", a);
	printf("%c%c%c%c%c%c%c%c\n",c,a,f,e,b,a,b,e);
	printf("number of bytes: %d.\n",  sizeof(c)+ sizeof(a) + sizeof(f) + sizeof(e) + sizeof(b) + sizeof(a) + sizeof(b) + sizeof(e) );
}