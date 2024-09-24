#include "basefile_processor.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;

static int verify_base_file(struct atc_list *statement_list) {
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

struct basefile_processor_result process_basefile(char *test_name) {
  struct basefile_processor_result result;
  result.is_success = 0;
  result.error_messages = NULL;
  result.warning_messages = NULL;
  result.base_file_statement_list = NULL;

  char filepath[1024];
  struct atc_list *base_file_statement_list;

  sprintf(filepath, "%s.at", test_name);
  yyin = fopen(filepath, "r");

  if (!yyin) {
    char error_message[1024];
    sprintf(error_message, "Cannot open file %s.at", test_name);
    result.error_messages =
        append_string_list(result.error_messages, error_message);
    return result;
  }

  if (yyparse()) {
    char error_message[32];
    sprintf(error_message, "Parse error!");
    result.error_messages =
        append_string_list(result.error_messages, error_message);
    return result;
  }
  base_file_statement_list = atc_statement_list;

  ATC_LIST_REVERSE(base_file_statement_list);

  if (!verify_base_file(base_file_statement_list)) {
    char error_message[64];
    sprintf(error_message, "Base file must contain only m4_include statements");
    result.error_messages =
        append_string_list(result.error_messages, error_message);
    return result;
  }

  result.is_success = 1;
  result.base_file_statement_list = ATC_LIST_NEXT(base_file_statement_list);
  return result;
}

void show_basefile_processor_messages(struct basefile_processor_result *result,
                                      FILE *error_stream,
                                      FILE *warning_stream) {
  if (result->warning_messages) {
    for (struct string_list *it = result->warning_messages; it; it = it->next) {
      fprintf(error_stream, "[Warning]: %s\n", it->value);
    }
  }
  if (result->error_messages) {
    for (struct string_list *it = result->error_messages; it; it = it->next) {
      fprintf(error_stream, "[Error]: %s\n", it->value);
    }
  }
}