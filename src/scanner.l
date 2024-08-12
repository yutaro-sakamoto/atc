%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int yywrap(void) {
    return 1;
}

int num_lines;
%}

ALPHABET [a-zA-Z_]
DIGIT [0-9]
LPAREN \(
RPAREN \)
LSQUARE \[
RSQUARE \]

%%

"AT_CHECK" {
    return AT_CHECK;
}

"AT_SETUP" {
    return AT_SETUP;
}

"AT_DATA" {
    return AT_DATA;
}

"AT_CLEANUP" {
    return AT_CLEANUP;
}

"AT_INIT" {
    return AT_INIT;
}

"m4_include" {
    return M4_INCLUDE;
}

{ALPHABET}({ALPHABET}|{DIGIT})* {
    yylval.text = strdup(yytext);
    return IDENTIFIER;
}

[ \t]+ ;
\n num_lines;

%%
