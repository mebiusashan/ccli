#include <stdio.h>
#include "ccli.h"

int force =0;
int abc = 0;
float cc = 0;
char * ss = "";

int rootRun ( int argc, const char **argv )
{
    printf ( "%d - %d - %f - %s \n", force, abc, cc, ss );
    printf ( "%d\n",argc );
    for ( int i=0; i<argc; i++ ) {
        printf ( "%s\n",argv[i] );
    }
    return 0;
}

int
main ( int argc, const char **argv )
{

    cmd_t root = set_root_cmd ( "ff","this is help with root command.","this is des.", "epilog string", &rootRun );
    cmd_t add = set_sub_cmd ( root, "add", "You can add articles, single \npages, Tweets and categories to the blog","Add content to the blog", "add epi",NULL );
    cmd_t rm = set_sub_cmd ( root, "rm", "You can delete articles, single pages, Tweets and categories in the blog","Add content to the blog", "rm epi",NULL );
    cmd_t page = set_sub_cmd ( add, "page", "The path of the markdown file needs to be set. If the - t parameter is set, \nthe parameter value is used as the article title. \nIf not set, the file name is used as the article \ntitle. At the same time, it must be set to the \nID of the classification to which the chapter belongs", "Select action page","add epi",NULL );

    set_opt ( root, CCLI_OPT_BOOL, 'f', "force", &force, "force option." );
    set_opt ( root, CCLI_OPT_INT, 'n', "num", &abc, "num option." );
    set_opt ( root, CCLI_OPT_FLOAT, 'w', "width", &cc, "width option." );
    set_opt ( root, CCLI_OPT_STRING, 'd', "des", &ss, "description option." );

    return  ccli_r ( root, argc, argv );
}
