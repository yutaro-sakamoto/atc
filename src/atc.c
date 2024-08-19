#include "parser.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
struct atc_list *atc_statement_list;

int main() {
  extern FILE *yyin;
  yyin = stdin;
  if (yyparse()) {
    fprintf(stderr, "Parse error!\n");
    exit(EXIT_FAILURE);
  }
  ATC_LIST_REVERSE(atc_statement_list);
  for (struct atc_list *it = atc_statement_list; it; it = ATC_LIST_NEXT(it)) {
    struct tree_common *statement = ATC_LIST_VALUE(it);
    if (IS_ATC_AT_CHECK(statement)) {
      struct atc_at_check *at_check = ATC_AT_CHECK(statement);
      printf("AT_CHECK: %s\n", at_check->command);
    } else if (IS_ATC_AT_SETUP(statement)) {
      struct atc_at_setup *at_setup = ATC_AT_SETUP(statement);
      printf("AT_SETUP: %s\n", at_setup->testsuite_name);
    } else if (IS_ATC_AT_DATA(statement)) {
      struct atc_at_data *at_data = ATC_AT_DATA(statement);
      printf("AT_DATA: %s\n", at_data->file_path);
    } else if (IS_ATC_AT_CLEANUP(statement)) {
      printf("AT_CLEANUP\n");
    } else if (IS_ATC_AT_INIT(statement)) {
      struct atc_at_init *at_init = ATC_AT_INIT(statement);
      printf("AT_INIT: %s\n", at_init->test_section_name);
    } else if (IS_ATC_M4_INCLUDE(statement)) {
      struct atc_m4_include *m4_include = ATC_M4_INCLUDE(statement);
      printf("M4_INCLUDE: %s\n", m4_include->file_name);
    }
  }
  return 0;
}