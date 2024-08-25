#include "parser.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

extern int yyparse(void);
struct atc_list *atc_statement_list;

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

int create_dir(char *dir) {
  // TODO: if necessary, dispatch the following process due to the OS
  return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

int run_test_file(char *test_name, char *file_name) {
  char filepath[1024];
  sprintf(filepath, "%s.src/%s", test_name, file_name);
  FILE *file = fopen(filepath, "r");
  if (!file) {
    fprintf(stderr, "Cannot open file %s\n", filepath);
    return 0;
  }
  char dirpath[1024];
  sprintf(dirpath, "%s.dir", test_name);
  if(create_dir(dirpath)) {
    fprintf(stderr, "Cannot create directory %s\n", dirpath);
    return 0;
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

  struct atc_list *m4_include_files = ATC_LIST_NEXT(atc_statement_list);
  for (struct atc_list *it = m4_include_files; it; it = ATC_LIST_NEXT(it)) {
    char *test_file_name = ATC_M4_INCLUDE(ATC_LIST_VALUE(it))->file_name;
    if (!run_test_file(argv[1], test_file_name)) {
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}