CC=c99
CFLAGS=-pedantic -std=c99

.PHONY: clean

mula: main.o parser.o eval.o
	$(CC) -o $@ $^

clean:
	rm -vf mula *.o
