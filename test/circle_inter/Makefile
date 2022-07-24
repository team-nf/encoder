app: build/cinter.o

build/cinter.o: main.c
	gcc -Wall -lm main.c -o build/cinter.o

run: build/cinter.o
	./build/cinter.o

clean:
	rm -rf build/* *.o 
