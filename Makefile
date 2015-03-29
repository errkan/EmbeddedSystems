all:
	gcc main.c -o main
	
indent:
	indent --linux-style main.c	
