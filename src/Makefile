CC = clang
CFLAGS = -g -Wall -Wextra -Werror -Wpedantic

all: encode decode

encode: encode.o huffman.o io.o code.o pq.o stack.o node.o
	$(CC) $(CFLAGS) -o encode $^

decode: decode.o huffman.o io.o code.o pq.o stack.o node.o
	$(CC) $(CFLAGS) -o decode $^

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

format:
	$(CC)-format -i -style=file *.[ch]

scan-build: clean
	scan-build make