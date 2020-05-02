#include <stdio.h>
#include "first.h"
int main()
{
	int a = 1;
	int b = 2;
	int c = add(a,b);
	printf("c = %d\n",c);
//	return a+b;
}

//bianyidongtaiku yiji
//gcc -shared -fPIC -Wall -o libMy.so first.c

