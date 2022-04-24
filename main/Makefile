CC=	clang
CFLAGS=	-c -nostdlib --target=wasm32

OBJS=   start.o

all:  $(OBJS)
	wasm-ld -o ../demo.wasm --no-entry \
	--export-all -z stack-size=1048576 --import-memory \
	start.o ../lib1/lib1.a ../debug/debug.a

clean:
	@rm -f *.o
	@rm -f ../demo.wasm

