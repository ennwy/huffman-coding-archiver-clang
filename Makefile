build-encode:
	gcc -g src/huffman/*.c src/utils/*.c src/encode.c -o encode

build-decode:
	gcc -g src/huffman/*.c src/utils/*.c src/decode.c -o decode

build: build-encode build-decode

