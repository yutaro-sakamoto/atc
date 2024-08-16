enum atc_type
{
    ATC_AT_CHECK,
    ATC_AT_SETUP,
    ATC_AT_DATA,
    ATC_AT_CLEANUP,
    ATC_AT_INIT,
    ATC_M4_INCLUDE,
};

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

struct atc_at_setup
{
    struct tree_common common;
    char *testsuite_name;
};

struct atc_at_data
{
    struct tree_common common;
    char *file_path;
    char *content;
};

struct atc_at_cleanup
{
    struct tree_common common;
};

struct atc_at_init
{
    struct tree_common common;
    char *test_section_name;
};

struct atc_m4_include
{
    struct tree_common common;
    char *file_name;
};