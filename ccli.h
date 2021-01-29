
#ifndef CCLI_H
#define CCLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define cmd_t unsigned int

enum ccli_option_type {
    CCLI_OPT_BOOL,
    CCLI_OPT_INT,
    CCLI_OPT_FLOAT,
    CCLI_OPT_STRING,
};

typedef int ccli_callback (int argc, const char **argv);

cmd_t
set_root_cmd(char *name, char *help, char *des, char *epilog, ccli_callback *callback);

cmd_t
set_sub_cmd( cmd_t parent, char *name, char *help, char *des, char *epilog, ccli_callback *callback);

void
set_opt(cmd_t cmd_id, enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help);

int
ccli_r(cmd_t root_id, int argc, const char **argv);

#ifdef __cplusplus
}
#endif

#endif
