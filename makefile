all: main.o functions.o functions.h
	gcc -o program main.o functions.o functions.h
  
main.o: main.c functions.h
	gcc -c main.c

functions.o: functions.c functions.c
	gcc -c functions.c functions.h

clean: 
	rm *.o

run:
	./program
  
