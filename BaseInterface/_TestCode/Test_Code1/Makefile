
Test1:test1.o first.o
	gcc test1.o first.o -o Test1
test1.o:test1.c first.c
	gcc -c test1.c -o test1.o
first.o:first.c
	gcc -c first.c -o first.o
My:
	gcc -shared -fPIC -Wall -o libMy.so first.c
clean:
	rm -rf *.o Test1
#gcc -shared -fPIC -Wall -o libMy.so first.c