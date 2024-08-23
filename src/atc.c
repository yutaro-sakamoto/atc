#include "parser.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
struct atc_list *atc_statement_list;

int verify_base_file(struct atc_list *statement_list) {
  struct atc_list *it = statement_list;
  if(!it || !IS_ATC_AT_INIT(it->value)) {
    return 0;
  }
  it = ATC_LIST_NEXT(it);
  for (; it; it = ATC_LIST_NEXT(it)) {
    if (!IS_ATC_M4_INCLUDE(it->value)) {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char *argv[]) {
  extern FILE *yyin;
  char filepath[1024];

  sprintf(filepath, "%s.at", argv[1]);
  yyin = fopen(filepath, "r");

  if (!yyin) {
    fprintf(stderr, "Cannot open file %s.at\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (yyparse()) {
    fprintf(stderr, "Parse error!\n");
    exit(EXIT_FAILURE);
  }
  ATC_LIST_REVERSE(atc_statement_list);

  if (!verify_base_file(atc_statement_list)) {
    fprintf(stderr, "Base file must contain only m4_include statements\n");
    exit(EXIT_FAILURE);
  }

  int m4_include_count = ATC_LIST_LENGTH(atc_statement_list) - 1;
  printf("Base file contains %d m4_include statements\n", m4_include_count);

  return 0;
}