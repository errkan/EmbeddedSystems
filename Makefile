all:
	gcc main.c -lm -o main
	
indent:
	indent --linux-style main.c	
