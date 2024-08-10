CC=gcc

all: atc

atc: atc.c lex.yy.c parser.tab.c
	$(CC) -o atc lex.yy.c parser.tab.c atc.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm atc lex.yy.* parser.tab.*

.PHONY: clean
