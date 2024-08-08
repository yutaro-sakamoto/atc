%{
#include <stdio.h>
#include <stdlib.h>

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
    printf("ID: %s\n", yytext);
}

[ \t]+ ;
\n num_lines;

%%

int main() {
    printf("Enter the input string:\n");
    yylex();
    printf("end of file\n");
    return 0;
}