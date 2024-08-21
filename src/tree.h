enum atc_type
{
    ATC_AT_CHECK,
    ATC_AT_SETUP,
    ATC_AT_DATA,
    ATC_AT_CLEANUP,
    ATC_AT_INIT,
    ATC_M4_INCLUDE,

    ATC_LIST,
};

#define ATC_TYPE(tree) (tree->type)
struct tree_common
{
    enum atc_type type;
};

struct atc_at_check
{
    struct tree_common common;
    char *command;
    char *expected_exit_code;
    char *expected_stdout;
    char *expected_stderr;
};

#define ATC_AT_CHECK(tree) ((struct atc_at_check *)tree)
#define IS_ATC_AT_CHECK(tree) (ATC_TYPE(tree) == ATC_AT_CHECK)

struct atc_at_setup
{
    struct tree_common common;
    char *testsuite_name;
};

#define ATC_AT_SETUP(tree) ((struct atc_at_setup *)tree)
#define IS_ATC_AT_SETUP(tree) (ATC_TYPE(tree) == ATC_AT_SETUP)

struct atc_at_data
{
    struct tree_common common;
    char *file_path;
    char *content;
};

#define ATC_AT_DATA(tree) ((struct atc_at_data *)tree)
#define IS_ATC_AT_DATA(tree) (ATC_TYPE(tree) == ATC_AT_DATA)

struct atc_at_cleanup
{
    struct tree_common common;
};

#define ATC_AT_CLEANUP(tree) ((struct atc_at_cleanup *)tree)
#define IS_ATC_AT_CLEANUP(tree) (ATC_TYPE(tree) == ATC_AT_CLEANUP)

struct atc_at_init
{
    struct tree_common common;
    char *test_section_name;
};

#define ATC_AT_INIT(tree) ((struct atc_at_init *)tree)
#define IS_ATC_AT_INIT(tree) (ATC_TYPE(tree) == ATC_AT_INIT)

struct atc_m4_include
{
    struct tree_common common;
    char *file_name;
};

#define ATC_M4_INCLUDE(tree) ((struct atc_m4_include *)tree)
#define IS_ATC_M4_INCLUDE(tree) (ATC_TYPE(tree) == ATC_M4_INCLUDE)

struct atc_list
{
    struct tree_common common;
    struct atc_list *next;
    struct tree_common *value;
};

#define ATC_LIST(tree) ((struct atc_list *)tree)
#define IS_ATC_LIST(tree) (ATC_TYPE(tree) == ATC_LIST)
#define ATC_LIST_NEXT(tree) (ATC_LIST(tree)->next)
#define ATC_LIST_VALUE(tree) (ATC_LIST(tree)->value)
#define ATC_LIST_APPEND(list, value) \
    list = atc_list_append(ATC_LIST(list), value);
#define ATC_LIST_REVERSE(list) \
    list = atc_list_reverse(ATC_LIST(list));
#define ATC_LIST_LENGTH(list) \
    atc_list_length(ATC_LIST(list));

extern struct atc_list* atc_list_append(struct atc_list *list, struct tree_common *value);
extern struct atc_list* atc_list_reverse(struct atc_list *list);
extern int atc_list_length(struct atc_list *list);

extern struct atc_list* atc_statement_list;