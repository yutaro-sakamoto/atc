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
{ALPHABET}({ALPHABET}|{DIGIT})* {
    yylval.text = strdup(yytext);
    return IDENTIFIER;
}

[ \t]+ ;
\n num_lines;

%%
