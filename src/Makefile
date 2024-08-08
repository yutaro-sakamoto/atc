CC=gcc

all: atc scanner

atc: atc.c
	$(CC) -o atc atc.c

scanner: lex.yy.c
	$(CC) -o scanner lex.yy.c

lex.yy.c: scanner.l
	flex scanner.l

