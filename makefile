all: main.o menu.o functions.o
	gcc -o program main.o menu.o functions.o
  
main.o: main.c menu.h
	gcc -c main.c

menu.o: menu.c menu.h
	gcc -c menu.c

functions.o: functions.c functions.h
	gcc -c functions.c

clean: 
	rm *.o *.gch

run:
	./program
  
