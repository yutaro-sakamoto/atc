CC=gcc

all: atc

atc: atc.c tree.o lex.yy.c parser.tab.c
	$(CC) -o atc tree.o lex.yy.c parser.tab.c atc.c

lex.yy.c: scanner.l
	flex scanner.l

tree: tree.c tree.h
	$(CC) -c tree.c

parser.tab.c: parser.y tree.h
	bison -d parser.y

clean:
	rm atc lex.yy.* parser.tab.*

test: atc
	echo 'AT_DATA ([hello], [world]) AT_SETUP([test])' | ./atc

.PHONY: clean test
