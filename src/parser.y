%{
#include <stdio.h>
#include <stdlib.h>    
int yylex();
void yyerror(char const *);
%}
%union {
    char* text;
}
%token AT_CHECK AT_SETUP AT_DATA AT_CLEANUP AT_INIT M4_INCLUDE
%token <text> IDENTIFIER
%start word_list

%%

word_list
:
| word_list word
;

word
: AT_CHECK {
    printf("keyword: AT_CHECK\n");
}
| AT_SETUP {
    printf("keyword: AT_SETUP\n");
}
| AT_DATA {
    printf("keyword: AT_DATA\n");
}
| AT_CLEANUP {
    printf("keyword: AT_CLEANUP\n");
}
| AT_INIT {
    printf("keyword: AT_INIT\n");
}
| M4_INCLUDE {
    printf("keyword: M4_INCLUDE\n");
}
| IDENTIFIER {
    printf("word: %s\n", $1);
}

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}