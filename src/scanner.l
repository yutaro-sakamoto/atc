%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int yywrap(void) {
    return 1;
}

int num_lines;
%}

%%

"(" {
    return '(';
}

")" {
    return ')';
}

"[" {
    return '[';
}

"]" {
    return ']';
}

"," {
    return ',';
}

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

[ \r\t]+ ;
\n ++num_lines;

\[[^\[\]]*\] {
    size_t len = strlen(yytext) - 1;
    yylval.text = malloc(len);
    memcpy(yylval.text, yytext + 1, len - 1);
    yytext[len] = '\0';
    return ARGUMENT;
}

%%
