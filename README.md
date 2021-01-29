# ccli

ccli - A command line arguments parsing library in C (compatible with C++).

## Description

This module is inspired by argparse.

Arguments parsing is common task in cli program, but traditional `getopt`
libraries are not easy to use. This library provides high-level arguments
parsing solutions.

The program defines what arguments it requires, and `argparse` will figure
out how to parse those out of `argc` and `argv`, it also automatically
generates help and usage messages and issues errors when users give the
program invalid arguments.

## Features

 - handles both optional and positional arguments
 - produces highly informative usage messages
 - issues errors when given invalid arguments
 - support subcommand

There are basically three types of options:

 - boolean options
 - options with mandatory argument
 - options with optional argument

There are basically two forms of options:

 - short option consist of one dash (`-`) and one alphanumeric character.
 - long option begin with two dashes (`--`) and some alphanumeric characters.

Short options may be bundled, e.g. `-a -b` can be specified as `-ab`.

Options are case-sensitive.

Options and non-option arguments can clearly be separated using the `--` option.

## Examples

```c
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

    return ccli_r ( root, argc, argv );
}
```

### Test Build

```
gcc test.c ccli.h ccli.c -o ccli
```

### Use

```
$ ./ccli -h
this is help with root command.

Usage:
  ff  [options]
  ff  [command]

Commands:
  add  Add content to the blog
  rm   Add content to the blog

Options:
  -f, --force            force option.
  -n, --num    <int>     num option.
  -w, --width  <float>   width option.
  -d, --des    <string>  description option.
  -h, --help             print help message.

epilog string
```

```
$ ./ccli -n5
0 - 5 - 0.000000 -  
0
```

```
$ ./ccli -f -d="this is des."
1 - 0 - 0.000000 - this is des. 
0
```
