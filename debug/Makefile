CC=	clang
CFLAGS=	-c -nostdlib --target=wasm32

OBJS=   debug.o

all:	debug.a

debug.a:  $(OBJS)
	llvm-ar rc debug.a $(OBJS)

clean:
	@rm -f *.o
	@rm -f debug.a

