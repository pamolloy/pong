CC=gcc
CFLAGS=-Wall -g -lcurses
OBJECTS=main.o paddle.o ball.o alarm.o

pong: $(OBJECTS)
	$(CC) $(CFLAGS) -o pong $(OBJECTS)

main.o : main.h paddle.h ball.h alarm.h
alarm.o : alarm.h
paddle.o : paddle.h
ball.o : ball.h

.PHONY : clean
clean:
	rm pong $(OBJECTS)
