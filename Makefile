OBJS = main.o
CFLAGS = -Wall
CC = gcc
OUTNAME = main
LIBS = -lncurses

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(OUTNAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o
