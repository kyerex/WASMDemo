CC=	clang
CFLAGS=	-nostdlib --target=wasm32 

OBJS=   add2.o add3.o

all:	lib1.a

lib1.a:  $(OBJS)
	llvm-ar rc lib1.a $(OBJS)

add3.o:	add3.wat
	wat2wasm -r add3.wat -o add3.o

clean:
	@rm -f *.o
	@rm -f lib1.a

