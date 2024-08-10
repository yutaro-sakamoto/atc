%{
#include <stdio.h>
#include <stdlib.h>    
int yylex();
void yyerror(char const *);
%}
%union {
    char* text;
}
%token <text> IDENTIFIER
%start id_list

%%

id_list
:  IDENTIFIER
{
    printf("Identifier: %s\n", $1);
}
|   id_list IDENTIFIER
{
    printf("Identifier: %s\n", $2);
}
;

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}