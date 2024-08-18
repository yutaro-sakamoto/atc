CC=gcc

all: atc

atc: atc.c tree.h lex.yy.c parser.tab.c
	$(CC) -o atc lex.yy.c parser.tab.c atc.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c: parser.y tree.h
	bison -d parser.y

clean:
	rm atc lex.yy.* parser.tab.*

test: atc
	#echo 'AT_CHECK () AT_SETUP () AT_DATA () AT_CLEANUP AT_INIT () m4_include ()' | ./atc
	echo 'AT_DATA ([hello], [world])' | ./atc

.PHONY: clean test
