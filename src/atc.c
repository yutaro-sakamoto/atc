#include "basefile_processor.h"
#include "fileio.h"
#include "parser.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yyparse(void);
struct atc_list *atc_statement_list;
extern FILE *yyin;

int execute_at_data(char *test_name, int test_case_count,
                    struct atc_at_data *at_data) {
  char filepath[1024];
  sprintf(filepath, "%s.dir/%d/%s", test_name, test_case_count,
          at_data->file_path);
  FILE *fp = fopen(filepath, "w");
  if (!fp) {
    fprintf(stderr, "Cannot open file %s\n", filepath);
    return 1;
  }
  fwrite(at_data->content, 1, strlen(at_data->content), fp);
  fclose(fp);
  return 0;
}

int execute_at_check(char *test_name, int test_case_count,
                     struct atc_at_check *at_check) {
  char command[1024];
  sprintf(command, "cd %s.dir/%d/ && %s", test_name, test_case_count,
          at_check->command);
  int return_code = system(command);
  if (return_code != at_check->expected_exit_code_int) {
    fprintf(stderr, "Command failed: %s\n", command);
  }
  return 0;
}

int execute_at_cleanup(char *test_name, int test_case_count) {
  char command[1024];
  sprintf(command, "rm -rf %s.dir/%d/*", test_name, test_case_count);
  int return_code = system(command);
  if (return_code != 0) {
    fprintf(stderr, "Command failed: %s\n", command);
  }
  return 0;
}

void execute_statement_list(char *test_name, int test_case_count,
                            struct atc_list *statement_list) {
  // char *test_case_name = NULL;
  for (struct atc_list *it = statement_list; it; it = ATC_LIST_NEXT(it)) {
    struct tree_common *statement = ATC_LIST_VALUE(it);
    if (IS_ATC_AT_SETUP(statement)) {
      // test_case_name = ATC_AT_SETUP(statement)->testsuite_name;
    } else if (IS_ATC_AT_DATA(statement)) {
      execute_at_data(test_name, test_case_count, ATC_AT_DATA(statement));
    } else if (IS_ATC_AT_CLEANUP(statement)) {
      execute_at_cleanup(test_name, test_case_count);
    } else if (IS_ATC_AT_INIT(statement)) {
      fprintf(stderr, "AT_INIT is not allowed in test files\n");
    } else if (IS_ATC_M4_INCLUDE(statement)) {
      fprintf(stderr, "M4_INCLUDE is not allowed in test files\n");
    } else if (IS_ATC_AT_CHECK(statement)) {
      execute_at_check(test_name, test_case_count, ATC_AT_CHECK(statement));
    }
  }
}

int create_test_temp_dir_root(char *test_name) {
  char dirpath[1024];
  sprintf(dirpath, "%s.dir", test_name);
  return create_dir(dirpath);
}

int remove_test_temp_dir_root(char *test_name) {
  char dirpath[1024];
  sprintf(dirpath, "%s.dir", test_name);
  return remove_dir(dirpath);
}

int create_test_temp_dir(char *test_name, int test_case_count) {
  char dirpath[1024];
  sprintf(dirpath, "%s.dir/%d", test_name, test_case_count);
  return create_dir(dirpath);
}

int remove_test_temp_dir(char *test_name, int test_case_count) {
  char dirpath[1024];
  sprintf(dirpath, "%s.dir/%d", test_name, test_case_count);
  return remove_dir(dirpath);
}

int validate_statement_list(struct atc_list *statement_list) {
  for (struct atc_list *it = statement_list; it; it = ATC_LIST_NEXT(it)) {
    struct tree_common *statement = ATC_LIST_VALUE(it);
    if (IS_ATC_AT_CHECK(statement)) {
      struct atc_at_check *at_check = ATC_AT_CHECK(statement);
      if (at_check->command == NULL) {
        return 0;
      }
      if (at_check->expected_exit_code == NULL ||
          strlen(at_check->expected_exit_code) == 0) {
        at_check->expected_exit_code_int = 0;
      } else if (strcmp(at_check->expected_exit_code, "0") == 0) {
        at_check->expected_exit_code_int = 0;
      } else {
        int parsed_int = atoi(at_check->expected_exit_code);
        if (parsed_int == 0) {
          return 0;
        }
        at_check->expected_exit_code_int = parsed_int;
      }
    }
  }
  return 1;
}

int run_test_file(char *test_name, char *file_name, int test_case_count) {
  char filepath[1024];
  sprintf(filepath, "%s.src/%s", test_name, file_name);
  atc_statement_list = NULL;
  yyin = fopen(filepath, "r");

  if (!yyin) {
    fprintf(stderr, "Cannot open file %s\n", filepath);
    return 0;
  }

  if (yyparse()) {
    fprintf(stderr, "Parse error!: %s\n", filepath);
    return 0;
  }

  if (!validate_statement_list(atc_statement_list)) {
    fprintf(stderr, "Invalid statement list\n");
    return 0;
  }

  create_test_temp_dir(test_name, test_case_count);
  execute_statement_list(test_name, test_case_count, atc_statement_list);
  remove_test_temp_dir(test_name, test_case_count);

  fclose(yyin);
  return 1;
}

int main(int argc, char *argv[]) {
  struct basefile_processor_result basefile_result = process_basefile(argv[1]);
  show_basefile_processor_messages(&basefile_result, stderr, stderr);

  if (!basefile_result.is_success) {
    return 1;
  }

  struct atc_list *m4_include_files = basefile_result.base_file_statement_list;

  char dirpath[1024];
  sprintf(dirpath, "%s.dir", argv[1]);
  if (!create_dir(dirpath)) {
    fprintf(stderr, "Cannot create directory %s\n", dirpath);
    return 0;
  }

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
  }

  int test_case_count = 1;
  for (struct atc_list *it = m4_include_files; it; it = ATC_LIST_NEXT(it)) {
    char *test_file_name = ATC_M4_INCLUDE(ATC_LIST_VALUE(it))->file_name;
    char *test_name = argv[1];
    create_test_temp_dir_root(test_name);
    if (!run_test_file(test_name, test_file_name, test_case_count)) {
      exit(EXIT_FAILURE);
    }
    remove_test_temp_dir_root(test_name);
  }

  if (!remove_dir(dirpath)) {
    fprintf(stderr, "Cannot remove directory %s\n", dirpath);
    return 0;
  }

  return 0;
}