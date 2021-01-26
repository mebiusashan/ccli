#include <stdio.h>

#include "ccli.h"

int
main(int argc, const char **argv)
{
    struct ccli_cmd *root = set_root_cmd("ff","this is help","this is des", "epi log string", NULL);
    struct ccli_cmd *add = set_sub_cmd(root, "add", "You can add articles, single \npages, Tweets and categories to the blog\n","add cmd", "add epi",NULL);
    struct ccli_cmd *rm = set_sub_cmd(root, "rm", "You can add articles, single \npages, Tweets and categories to the blog\n","add cmd", "add epi",NULL);
    struct ccli_cmd *page = set_sub_cmd(add, "page", "You can add articles, single \npages, Tweets and categories to the blog\n", "page cmd","add epi",NULL);
    
    int force =0;
    set_opt(root, CCLI_OPT_BOOL, 'f', "force", &force, "force run cmd."  );
    
    ccli_r(root, argc, argv);
    
    /*
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
    */
    //printf("%d\n", argc);
    return 0;
}
