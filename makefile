all: scheduler clean

#get all the c files
SRC=$(wildcard *.c)
#create the name of the needed object file from the c files' names
OBJ=$(patsubst %.c,%.o,$(SRC))

scheduler: $(OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc -c $^
	
clean:
	rm -f *.o



