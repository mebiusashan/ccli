#include <stdio.h>

#include "ccli.h"

int
main(int argc, const char **argv)
{
    int force = 0;
    
    struct ccli_option opts[] = {
        CCLI_BUILT_OPT_BOOL('f',"force", &force, "force to do"),
        CCLI_BUILT_OPT_BOOL('t',"tt", &force, "force to do"),
    };
    
    struct ccli_command cmds[] = {
        {"add","添加描述","添加底部描述"},
        {"rm","rm描述","rm底部描述"},
    };
    
    struct ccli_command cmd = {
        "test"
        "这是描述",
        "底部的描述",
    };
    cmd.options = opts;
    cmd.sub_commands = cmds;
    
    ccli_run(&cmd, argc, argv);
    //printf("%d\n", argc);
    return 0;
}
