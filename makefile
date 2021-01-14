all: main.o menu.o menu.h
	gcc -o program main.o menu.o menu.h
  
main.o: main.c menu.h
	gcc -c main.c

menu.o: menu.c menu.c
	gcc -c menu.c menu.h

clean: 
	rm *.o *.gch

run:
	./program
  
