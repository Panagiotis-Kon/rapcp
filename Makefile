## makefile

HEADER  = Header.h
OBJS 	= Main.o Functions.o DataFunctions.o
SOURCE	= Main.c Functions.c DataFunctions.c
OUT  	= rapcp
CC	= gcc

.PHONY: execute
execute: rapcp
	./rapcp -v Source Target

rapcp:  Main.o Functions.o DataFunctions.o
	$(CC) -o $(OUT) $(OBJS)

rapcp64:  Main.o Functions.o DataFunctions.o
	$(CC) -o $(OUT) $(OBJS) -D_FILE_OFFSET_BITS=64

#seperate compilation
Main.o: Main.c
	$(CC) -c Main.c
   
Functions.o: Functions.c
	$(CC) -c Functions.c

DataFunctions.o: DataFunctions.c
	$(CC) -c DataFunctions.c 

#clean
clean:
	rm -f $(OUT) $(OBJS)

