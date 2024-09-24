#include "tree.h"
#include <stdio.h>

struct basefile_processor_result {
  int is_success;
  struct string_list *error_messages;
  struct string_list *warning_messages;
  struct atc_list *base_file_statement_list;
};

struct basefile_processor_result process_basefile(char *test_name);
void show_basefile_processor_messages(
    struct basefile_processor_result *messages, FILE *error_stream,
    FILE *warning_stream);