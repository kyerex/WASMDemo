CC=	gcc
CPP=	g++
CPPFLAGS= -g -O -Wall -I.


OBJS=	ServerLog.o \
	Sock2.o \
	loadfile.o \
	do.o \
	server.o

ifeq ($(OS),Windows_NT)
LIBS2=-lws2_32
STATIC=-static
else
LIBS2=
STATIC=
endif

all:	$(OBJS) 
	g++ $(STATIC) -o server $(OBJS) $(LIBS2)  
	(cd lib1;make)
	(cd debug;make)
	(cd main;make)
clean:
	@rm -f *.o
	@rm -f server.exe
	@rm -f server
	(cd lib1;make clean)
	(cd debug;make clean)
	(cd main;make clean)
