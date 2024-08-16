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
%token <text> ARGUMENT
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
: AT_CHECK '(' ARGUMENT ')'
{
    printf("AT_CHECK statement 1\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ')'
{
    printf("AT_CHECK statement 2\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    printf("AT_CHECK statement 3\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    printf("AT_CHECK statement 4\n");
}

at_setup_statement
: AT_SETUP '(' ARGUMENT ')'
{
    printf("AT_SETUP statement\n");
}

at_data_statement
: AT_DATA '(' ARGUMENT ',' ARGUMENT ')'
{
    printf("AT_DATA statement\n");
}

at_cleanup_statement
: AT_CLEANUP
{
    printf("AT_CLEANUP statement\n");
}

at_init_statement
: AT_INIT '(' ARGUMENT')'
{
    printf("AT_INIT statement\n");
}

m4_include_statement
: M4_INCLUDE '(' ARGUMENT')'
{
    printf("M4_INCLUDE statement\n");
}

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}