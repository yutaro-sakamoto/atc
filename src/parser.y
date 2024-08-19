%{
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
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
{
    ATC_LIST_APPEND(atc_statement_list, $2);
}
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
    atc_at_check->common.type = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = NULL;
    atc_at_check->expected_stdout = NULL;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    atc_at_check->common.type = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = NULL;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    atc_at_check->common.type = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = $7;
    atc_at_check->expected_stderr = NULL;
    $$ = (struct tree_common*)atc_at_check;
}
| AT_CHECK '(' ARGUMENT ',' ARGUMENT ',' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_check* atc_at_check = malloc(sizeof(struct atc_at_check));
    atc_at_check->common.type = ATC_AT_CHECK;
    atc_at_check->command = $3;
    atc_at_check->expected_exit_code = $5;
    atc_at_check->expected_stdout = $7;
    atc_at_check->expected_stderr = $9;
    $$ = (struct tree_common*)atc_at_check;
}

at_setup_statement
: AT_SETUP '(' ARGUMENT ')'
{
    struct atc_at_setup* atc_at_setup = malloc(sizeof(struct atc_at_setup));
    atc_at_setup->common.type = ATC_AT_SETUP;
    atc_at_setup->testsuite_name = $3;
    $$ = (struct tree_common*)atc_at_setup;
}

at_data_statement
: AT_DATA '(' ARGUMENT ',' ARGUMENT ')'
{
    struct atc_at_data* atc_at_data = malloc(sizeof(struct atc_at_data));
    atc_at_data->common.type = ATC_AT_DATA;
    atc_at_data->file_path = $3;
    atc_at_data->content = $5;
    $$ = (struct tree_common*)atc_at_data;
}

at_cleanup_statement
: AT_CLEANUP
{
    struct atc_at_cleanup* atc_at_cleanup = malloc(sizeof(struct atc_at_cleanup));
    atc_at_cleanup->common.type = ATC_AT_CLEANUP;
    $$ = (struct tree_common*)atc_at_cleanup;
}

at_init_statement
: AT_INIT '(' ARGUMENT')'
{
    struct atc_at_init* atc_at_init = malloc(sizeof(struct atc_at_init));
    atc_at_init->common.type = ATC_AT_INIT;
    atc_at_init->test_section_name = $3;
    $$ = (struct tree_common*)atc_at_init;
}

m4_include_statement
: M4_INCLUDE '(' ARGUMENT')'
{
    struct atc_m4_include* atc_m4_include = malloc(sizeof(struct atc_m4_include));
    atc_m4_include->common.type = ATC_M4_INCLUDE;
    atc_m4_include->file_name = $3;
    $$ = (struct tree_common*)atc_m4_include;
}

%%
void
yyerror(char const *s)
{
    fprintf(stderr, "Error: %s\n", s);
}