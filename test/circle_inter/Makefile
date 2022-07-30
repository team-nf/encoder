CFLAGS=-Wall -g -lm
MAINPROG=main.c

app: build/cinter.o

build/cinter.o: main.c cinter.h impl/cinter-impl.h config.h
	gcc $(CFLAGS) $(MAINPROG) -o build/cinter.o

run: build/cinter.o 
	./build/cinter.o

clean:
	rm -rf build/* *.o 
