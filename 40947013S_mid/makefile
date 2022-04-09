all: mid01.o mystring.c mid02.o mid03.o
	gcc mystring.c mid01.o -o mid01
	gcc mid02.o -o mid02 -lm
	gcc mid03.o -o mid03 -lm
	
mid01: mid01.c
	gcc -c mid01.c -o mid01.o 
	gcc -shared mid01.o -o mid01.so

mid02: mid02.c
	gcc -c mid02.c -o mid02.o 
	gcc -shared mid02.o -o mid02.so

mid03: mid03.c
	gcc -c mid03.c -o mid03.o -lm
	gcc -shared mid03.o -o mid03.so

