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
%token LPAREN RPAREN
%start statement_list

%%

statement_list
:
| statement_list statement
;

statement
: at_check_statement
| at_setup_statement
| at_data_statement
| at_cleanup_statement
| at_init_statement
| m4_include_statement

at_check_statement
: AT_CHECK '(' ')'
{
    printf("AT_CHECK statement\n");
}

at_setup_statement
: AT_SETUP '(' ')'
{
    printf("AT_SETUP statement\n");
}

at_data_statement
: AT_DATA '(' ')'
{
    printf("AT_DATA statement\n");
}

at_cleanup_statement
: AT_CLEANUP
{
    printf("AT_CLEANUP statement\n");
}

at_init_statement
: AT_INIT '(' ')'
{
    printf("AT_INIT statement\n");
}

m4_include_statement
: M4_INCLUDE '(' ')'
{
    printf("M4_INCLUDE statement\n");
}

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}