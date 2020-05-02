#include "sec.h"
#include <stdio.h>
int main()
{
	int a = 1;
	int b = 2;
	int c = add2(a,b);
	printf("c = %d\n",c);
//	return a+b;
}


gcc -L. -lsec -L. -lfirst -lpthread -O2 -I. -I/include  -c test.c -o test.o
