#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);

int main() {
  extern FILE *yyin;
  yyin = stdin;
  if (yyparse()) {
    fprintf(stderr, "Parse error!\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}