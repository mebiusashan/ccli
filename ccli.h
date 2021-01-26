
#ifndef CCLI_H
#define CCLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct ccli_cmd CCLI_CMD;
typedef struct ccli_opt CCLI_OPT;

typedef int ccli_callback (int argc, const char **argv);

enum ccli_option_type {
    CCLI_OPT_BOOL,
    CCLI_OPT_INT,
    CCLI_OPT_FLOAT,
    CCLI_OPT_STRING,
};

struct  ccli_opt {
    enum ccli_option_type type;
    char short_name;
    const char *long_name;
    CCLI_OPT *prev_opt;
    CCLI_OPT *next_opt;
    void *value;
    const char *help;
    intptr_t data;
};

struct ccli_cmd {
    const char *name;
    const char *help;
    const char *description;
    const char *epilog;
    ccli_callback *callback;
    CCLI_OPT *first_opt;
    CCLI_OPT *last_opt;
    CCLI_CMD *first_sub_cmd;
    CCLI_CMD *last_sub_cmd;
    CCLI_CMD *parent_cmd;
    CCLI_CMD *prev_cmd;
    CCLI_CMD *next_cmd;
};

CCLI_CMD*
set_root_cmd(char *name, char *help, char *des, char *epilog, ccli_callback *callback);

CCLI_CMD*
set_sub_cmd( CCLI_CMD *root, char *name, char *help, char *des, char *epilog, ccli_callback *callback);

void
set_opt(CCLI_CMD *cmd, enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help);

int
ccli_r(CCLI_CMD *root, int argc, const char **argv);


#ifdef __cplusplus
}
#endif

#endif
