CC=gcc

all: atc

atc: atc.c tree.o lex.yy.c parser.tab.c fileio.o
	$(CC) -o atc tree.o lex.yy.c parser.tab.c atc.c fileio.o

fileio.o: fileio.c fileio.h
	$(CC) -c fileio.c

lex.yy.c: scanner.l
	flex scanner.l

tree: tree.c tree.h
	$(CC) -c tree.c

parser.tab.c: parser.y tree.h
	bison -d parser.y

clean:
	rm atc lex.yy.* parser.tab.*

test: atc
	echo 'AT_DATA ([hello], [world])' \
	'AT_SETUP([test])' \
	'AT_CHECK([echo hello])'| \
	'AT_CHECK([echo hello], [0], [stdout text])'| \
	'AT_CHECK([echo hello], [0], [stdout text], [stderr text])'| \
	'AT_INIT([test section])' \
	'AT_CLEANUP' \
	'M4_INCLUDE([file.at])' \
	| ./atc

.PHONY: clean test
