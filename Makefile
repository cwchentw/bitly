PROGRAM=bitly
CFLAGS=-Wall
CPPFLAGS=
LDFLAGS=
LIBS=-lcurl -ljson-c
CC=gcc
RM=rm
RMFLAGS=-rf

$(PROGRAM):
	$(CC) $(PROGRAM).c -o $(PROGRAM) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS)

clean:
	$(RM) $(RMFLAGS) $(PROGRAM)
