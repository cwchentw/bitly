PROGRAM=bitly
CFLAGS=-g -Wall
CPPFLAGS=
LDFLAGS=
LIBS=-lcurl -ljson-c
CC=gcc
RM=rm
RMFLAGS=-rf

$(PROGRAM): $(PROGRAM).o
	$(CC) $(PROGRAM).o -o $(PROGRAM) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS)

$(PROGRAM).o:
	$(CC) -c $(PROGRAM).c 

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	$(RM) $(RMFLAGS) $(PROGRAM) nul *.o
