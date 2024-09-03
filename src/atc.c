#include "fileio.h"
#include "parser.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
struct atc_list *atc_statement_list;
extern FILE *yyin;

int verify_base_file(struct atc_list *statement_list) {
  struct atc_list *it = statement_list;
  if (!it || !IS_ATC_AT_INIT(it->value)) {
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

void execute_statement_list(struct atc_list *statement_list) {
  for (struct atc_list *it = statement_list; it; it = ATC_LIST_NEXT(it)) {
    struct tree_common *statement = ATC_LIST_VALUE(it);
    if (IS_ATC_AT_SETUP(statement)) {
      printf("AT_SETUP\n");
    } else if (IS_ATC_AT_DATA(statement)) {
      printf("AT_DATA\n");
    } else if (IS_ATC_AT_CLEANUP(statement)) {
      printf("AT_CLEANUP\n");
    } else if (IS_ATC_AT_INIT(statement)) {
      printf("AT_INIT\n");
    } else if (IS_ATC_M4_INCLUDE(statement)) {
      printf("M4_INCLUDE\n");
    }
  }
}

int run_test_file(char *test_name, char *file_name) {
  char filepath[1024];
  sprintf(filepath, "%s.src/%s", test_name, file_name);
  yyin = fopen(filepath, "r");
  if (!yyin) {
    fprintf(stderr, "Cannot open file %s\n", filepath);
    return 0;
  }
  if (yyparse()) {
    fprintf(stderr, "Parse error!: %s\n", filepath);
    return 0;
  }
  execute_statement_list(atc_statement_list);
  fclose(yyin);
  return 1;
}

int main(int argc, char *argv[]) {
  char filepath[1024];
  struct atc_list *base_file_statement_list;

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
  base_file_statement_list = atc_statement_list;

  ATC_LIST_REVERSE(base_file_statement_list);

  if (!verify_base_file(base_file_statement_list)) {
    fprintf(stderr, "Base file must contain only m4_include statements\n");
    exit(EXIT_FAILURE);
  }

  struct atc_list *m4_include_files = ATC_LIST_NEXT(base_file_statement_list);

  char dirpath[1024];
  sprintf(dirpath, "%s.dir", argv[1]);
  if (!create_dir(dirpath)) {
    fprintf(stderr, "Cannot create directory %s\n", dirpath);
    return 0;
  }

  int test_case_count = 0;
  for (struct atc_list *it = m4_include_files; it; it = ATC_LIST_NEXT(it)) {
    char test_file_path[1024];
    sprintf(test_file_path, "%s.src/%s", argv[1],
            ATC_M4_INCLUDE(ATC_LIST_VALUE(it))->file_name);
    yyin = fopen(test_file_path, "r");
    if (!yyin) {
      fprintf(stderr, "Cannot open file %s\n", test_file_path);
      return 0;
    }
    if (yyparse()) {
      fprintf(stderr, "Parse error!: %s\n", test_file_path);
      exit(EXIT_FAILURE);
    }
    test_case_count += ATC_LIST_LENGTH(atc_statement_list);
  }

  printf("[dbg] test_case_count: %d\n", test_case_count);

  for (struct atc_list *it = m4_include_files; it; it = ATC_LIST_NEXT(it)) {
    char *test_file_name = ATC_M4_INCLUDE(ATC_LIST_VALUE(it))->file_name;
    if (!run_test_file(argv[1], test_file_name)) {
      exit(EXIT_FAILURE);
    }
  }

  if (!remove_dir(dirpath)) {
    fprintf(stderr, "Cannot remove directory %s\n", dirpath);
    return 0;
  }
  return 0;
}