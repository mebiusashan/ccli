
#ifndef CCLI_H
#define CCLI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>

struct ccli_cmd;
struct ccli_opt;

typedef struct ccli_cmd CCLI_CMD;
typedef struct ccli_opt CCLI_OPT;
    
typedef int ccli_callback (struct ccli_cmd *self, const struct ccli_opt *option);

enum ccli_option_type {
    CCLI_OPT_BOOL,
    CCLI_OPT_INT,
    CCLI_OPT_FLOAT,
    CCLI_OPT_STRING,
};

 struct  ccli_opt{
    enum ccli_option_type type;
    char short_name;
    const char *long_name;
    struct ccli_opt *prev_opt;
    struct ccli_opt *next_opt;
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
    struct ccli_opt *first_opt;
    struct ccli_opt *last_opt;
    struct ccli_cmd *first_sub_cmd;
    struct ccli_cmd *last_sub_cmd;
    CCLI_CMD *parent_cmd;
    struct ccli_cmd *prev_cmd;
    struct ccli_cmd *next_cmd;
};

struct ccli_cmd*
set_root_cmd(char *name, char *help, char *des, char *epilog, ccli_callback *callback);

struct ccli_cmd*
set_sub_cmd( struct ccli_cmd *root, char *name, char *help, char *des, char *epilog, ccli_callback *callback);

void
set_opt(CCLI_CMD *cmd, enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help);

void
ccli_r(struct ccli_cmd *root, int argc, const char **argv);
    
struct ccli_command;
struct ccli_option;


struct ccli_option {
    enum ccli_option_type type;
    const char short_name;
    const char *long_name;
    void *value;
    const char *help;
    intptr_t data;
};

struct ccli_command {
    const char *name;
    const char *description;
    const char *epilog;
    //ccli_callback *callback;
    const struct ccli_option *options;
    const struct ccli_command *sub_commands;
};

#define CCLI_BUILT_COMMAND(...) { __VA_ARGS__ }
#define CCLI_BUILT_OPT_BOOL(...) {CCLI_OPT_BOOL, __VA_ARGS__ }

void ccli_run(struct ccli_command *cmd, int argc, const char **argv);

#ifdef __cplusplus
}
#endif

#endif
