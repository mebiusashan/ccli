#include <stdio.h>
#include <string.h>

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
    set_opt(root, CCLI_OPT_BOOL, 'f', "force", &force, "force run cmd."  );
    set_opt(root, CCLI_OPT_BOOL, 'f', "force", &force, "force run cmd."  );
    
    ccli_r(root, argc, argv);
    
    return 0;
}
