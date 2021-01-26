#include <stdio.h>
#include <string.h>

#include "ccli.h"

int force =0;
int abc = 0;
float cc = 0;
char * ss = "";

int rootRun(int argc, const char **argv)
{
    printf("%d - %d - %f - %s \n", force, abc, cc, ss);
    for(int i=0; i<argc; i++) {
        //      printf("%s\n",argv[i]);
    }
    return 0;
}

int
main(int argc, const char **argv)
{

    struct ccli_cmd *root = set_root_cmd("ff","this is help","this is des", "epi log string", &rootRun);
    struct ccli_cmd *add = set_sub_cmd(root, "add", "You can add articles, single \npages, Tweets and categories to the blog\n","add cmd", "add epi",NULL);
    struct ccli_cmd *rm = set_sub_cmd(root, "rm", "You can add articles, single \npages, Tweets and categories to the blog\n","add cmd", "add epi",NULL);
    struct ccli_cmd *page = set_sub_cmd(add, "page", "You can add articles, single \npages, Tweets and categories to the blog\n", "page cmd","add epi",NULL);

    set_opt(root, CCLI_OPT_BOOL, 'f', "force", &force, "force run cmd."  );
    set_opt(root, CCLI_OPT_INT, 'a', "abc", &abc, "force run cmd."  );
    set_opt(root, CCLI_OPT_FLOAT, 'c', "cc", &cc, "force run cmd."  );
    set_opt(root, CCLI_OPT_STRING, 's', "ss", &ss, "force run cmd."  );

    return ccli_r(root, argc, argv);


    return 0;
}
