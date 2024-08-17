%{
#include <stdio.h>
#include <stdlib.h>
#include "atc.h"
int yylex();
void yyerror(char const *);
%}
%union {
    char* text;
    struct tree_common* tree;
}

%token AT_CHECK AT_SETUP AT_DATA AT_CLEANUP AT_INIT M4_INCLUDE
%token <text> ARGUMENT
%type <tree> at_check_statement at_setup_statement at_data_statement at_cleanup_statement at_init_statement m4_include_statement statement statement_list
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
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    ATC_TYPE(atc_at_check) = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = NULL;
    atc_at_check->expected_stdout = NULL;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
    printf("AT_CHECK statement 1\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    ATC_TYPE(atc_at_check) = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = NULL;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
    printf("AT_CHECK statement 2\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    ATC_TYPE(atc_at_check) = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = $7;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
    printf("AT_CHECK statement 3\n");
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    ATC_TYPE(atc_at_check) = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = $7;
    atc_at_check->expected_stderr = $9;
    $$ = (struct tree_common*)atc_at_check;
    printf("AT_CHECK statement 4\n");
}

at_setup_statement
: AT_SETUP '(' ARGUMENT ')'
{
    struct atc_at_setup* atc_at_setup = malloc(sizeof(struct atc_at_setup));
    ATC_TYPE(atc_at_setup) = ATC_AT_SETUP;
    atc_at_setup->testsuite_name = $3;
    $$ = (struct tree_common*)atc_at_setup;
    printf("AT_SETUP statement\n");
}

at_data_statement
: AT_DATA '(' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_data* atc_at_data = malloc(sizeof(struct atc_at_data));
    ATC_TYPE(atc_at_data) = ATC_AT_DATA;
    atc_at_data->file_path = $3;
    atc_at_data->content = $5;
    $$ = (struct tree_common*)atc_at_data;
    printf("AT_DATA statement\n");
}

at_cleanup_statement
: AT_CLEANUP
{
    struct atc_at_cleanup* atc_at_cleanup = malloc(sizeof(struct atc_at_cleanup));
    ATC_TYPE(atc_at_cleanup) = ATC_AT_CLEANUP;
    $$ = (struct tree_common*)atc_at_cleanup;
    printf("AT_CLEANUP statement\n");
}

at_init_statement
: AT_INIT '(' ARGUMENT')'
{
    struct atc_at_init* atc_at_init = malloc(sizeof(struct atc_at_init));
    ATC_TYPE(atc_at_init) = ATC_AT_INIT;
    atc_at_init->test_section_name = $3;
    $$ = (struct tree_common*)atc_at_init;
    printf("AT_INIT statement\n");
}

m4_include_statement
: M4_INCLUDE '(' ARGUMENT')'
{
    struct atc_m4_include* atc_m4_include = malloc(sizeof(struct atc_m4_include));
    ATC_TYPE(atc_m4_include) = ATC_M4_INCLUDE;
    atc_m4_include->file_name = $3;
    $$ = (struct tree_common*)atc_m4_include;
    printf("M4_INCLUDE statement\n");
}

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}