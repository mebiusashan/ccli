
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "ccli.h"


CCLI_CMD*
create_cmd(char *name,char *help, char *des, char *epilog, ccli_callback *callback)
{
    CCLI_CMD *cmd = (CCLI_CMD*)malloc(sizeof(CCLI_CMD));
    cmd->name = name;
    cmd->help = help;
    cmd->description = des;
    cmd->epilog = epilog;
    cmd->callback = callback;
    cmd->first_opt = NULL;
    cmd->last_opt = NULL;
    cmd->first_sub_cmd = NULL;
    cmd->last_sub_cmd = NULL;
    cmd->parent_cmd = NULL;
    cmd->next_cmd = NULL;
    //cmd->prev_cmd = NULL;
    return cmd;
}

CCLI_CMD*
set_root_cmd(char *name, char *help,char *des, char *epilog, ccli_callback *callback)
{
    return create_cmd(name, help, des, epilog, callback);
}

CCLI_CMD*
set_sub_cmd( CCLI_CMD *root, char *name, char *help, char *des, char *epilog, ccli_callback *callback)
{
    CCLI_CMD *sub_cmd = create_cmd(name, help, des, epilog, callback);
    sub_cmd->parent_cmd = root;
    if(root!=NULL)
    {
        if(root->first_sub_cmd==NULL)
        {
            root->first_sub_cmd=sub_cmd;
        }

        if(root->last_sub_cmd==NULL)
        {
            root->last_sub_cmd = sub_cmd;
            if(root->first_sub_cmd!=sub_cmd)
            {
                root->first_sub_cmd->next_cmd = root->last_sub_cmd;
                //root->last_sub_cmd->prev_cmd = root->first_sub_cmd;
            }
        } else {
            root->last_sub_cmd->next_cmd = sub_cmd;
            //sub_cmd->prev_cmd = root->last_sub_cmd;
            root->last_sub_cmd = sub_cmd;
        }
    }
    return sub_cmd;
}

CCLI_OPT*
create_opt(enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help)
{
    CCLI_OPT *opt = (CCLI_OPT*)malloc(sizeof(CCLI_OPT));
    opt->type = type;
    opt->short_name = short_name;
    opt->long_name = long_name;
    //opt->prev_opt = NULL;
    opt->next_opt = NULL;
    opt->value = value;
    opt->help = help;
    return opt;
}

void
set_opt(CCLI_CMD *cmd, enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help)
{
    CCLI_OPT *opt = create_opt(type, short_name, long_name, value, help);
    if(cmd->first_opt==NULL) {
        cmd->first_opt = opt;
    }

    if(cmd->last_opt==NULL)
    {
        cmd->last_opt = opt;
        if(cmd->first_opt!=opt)
        {
            cmd->first_opt->next_opt = opt;
            //opt->prev_opt = cmd->first_opt;
        }
    } else {
        cmd->last_opt->next_opt = opt;
        //opt->prev_opt = cmd->last_opt;
        cmd->last_opt = opt;
    }
}

#define OPTION_CHAR_WIDTH 8
#define PRINTF_OPT(...) printf("  -%c, --%-*s%-*s%s\n",__VA_ARGS__);

void
ccli_help(CCLI_CMD *cmd)
{
    printf("%s\n\nUsage:\n", cmd->help);
    char *cmds = "";
    CCLI_CMD *curCmd = cmd;
    while(curCmd)
    {
        char *oldcmds = (char*)malloc(strlen(cmds)+1);
        memcpy(oldcmds, cmds, strlen(cmds)+1);
        cmds = (char*)malloc(sizeof(cmds)+strlen(curCmd->name)+1);
        strcat(cmds, curCmd->name);
        strcat(cmds, " ");
        strcat(cmds, oldcmds);
        curCmd = curCmd->parent_cmd;
    }
    curCmd = cmd;
    if(curCmd->first_opt!=NULL)
    {
        printf("  %s [options]\n", cmds);
    }
    if(curCmd->first_sub_cmd!=NULL)
    {
        printf("  %s [command]\n", cmds);
    }
    /*
    if(curCmd->first_opt==NULL && curCmd->first_sub_cmd==NULL)
    {
        printf("  %s [args]\n", cmds);
    }
    */

    if(curCmd->first_sub_cmd!=NULL)
    {
        printf("\nCommands:\n");
        int width = 2;
        curCmd = cmd->first_sub_cmd;
        while(curCmd) {
            width = width<strlen(curCmd->name)?strlen(curCmd->name)+2:width;
            curCmd = curCmd->next_cmd;
        }
        curCmd = cmd->first_sub_cmd;
        while(curCmd) {
            int w = width-strlen(curCmd->name);
            printf("  %-*s  %s\n",w, curCmd->name, curCmd->description);
            curCmd = curCmd->next_cmd;
        }
    }

    printf("\nOptions:\n");
    int width = 2;
    if(cmd->first_opt!=NULL)
    {
        CCLI_OPT* curOpt = cmd->first_opt;
        while(curOpt) {
            width = width<strlen(curOpt->long_name)?strlen(curOpt->long_name)+2:width;
            curOpt = curOpt->next_opt;
        }
        curOpt = cmd->first_opt;
        while(curOpt) {
            switch(curOpt->type)
            {
            case CCLI_OPT_INT:
                PRINTF_OPT(curOpt->short_name, width, curOpt->long_name,OPTION_CHAR_WIDTH, "int", curOpt->help);
                break;
            case CCLI_OPT_FLOAT:
                PRINTF_OPT(curOpt->short_name, width, curOpt->long_name,OPTION_CHAR_WIDTH, "float", curOpt->help);
                break;
            case CCLI_OPT_STRING:
                PRINTF_OPT(curOpt->short_name, width, curOpt->long_name,OPTION_CHAR_WIDTH, "string", curOpt->help);
                break;
            default:
                PRINTF_OPT(curOpt->short_name, width, curOpt->long_name,OPTION_CHAR_WIDTH, "", curOpt->help);
            }
            curOpt = curOpt->next_opt;
        }
    }
    PRINTF_OPT('h', width, "help",OPTION_CHAR_WIDTH, "", "print help message.");

    if(cmd->epilog!=NULL)
    {
        printf("\n%s\n", cmd->epilog);
    }
}

CCLI_CMD *
ccli_r_cmd(CCLI_CMD *cmd, int *index, int argc, const char **argv)
{
    if(*index>=argc)
    {
        return cmd;
    }

    const char *str = argv[*index];
    CCLI_CMD *curCmd = cmd->first_sub_cmd;
    while(curCmd)
    {
        //printf("cmd: %s\n", curCmd->name);
        if(strcoll(curCmd->name,str)==0)
        {
            (*index)++;
            return ccli_r_cmd(curCmd, index, argc, argv);
        }
        curCmd = curCmd->next_cmd;
    }
    return cmd;
}

#define CCLI_LONG_NAME_PREFIX 2
#define CCLI_SHORT_NAME_PREFIX 1

void
ccli_r_opt(CCLI_CMD *cmd, int *index, int argc, const char **argv)
{
    for(int i=*index; i<argc; i++)
    {
        *index = i;
        if(i>=argc) {
            return;
        }
        const char *str = argv[i];
        if(str[0] != '-' || strlen(str)<CCLI_LONG_NAME_PREFIX)
        {
            return;
        }
        CCLI_OPT *opt = cmd->first_opt;

        if(!opt)
        {
            if(strcoll(str, "--help")==0||(str[0]=='-' && str[1]=='h'))
            {
                ccli_help(cmd);
                exit(0);
            }
        }

        while(opt)
        {
            if(str[1]=='-')
            {
                if(strlen(str)>=(strlen(opt->long_name)+CCLI_LONG_NAME_PREFIX))
                {
                    int l = strlen(opt->long_name);
                    char *c = (char *)malloc(l);
                    const char *s = str+CCLI_LONG_NAME_PREFIX;
                    memcpy(c, s, l);
                    if(strcoll(c, "help")==0)
                    {
                        ccli_help(cmd);
                        exit(0);
                    }
                    //printf("%s - %s -- %d\n", s, opt->long_name, strcoll(c, opt->long_name));
                    if(strcoll(c, opt->long_name)==0)
                    {
                        switch(opt->type)
                        {
                        case CCLI_OPT_BOOL:
                            *(int *)opt->value = 1;
                            break;
                        case CCLI_OPT_INT:
                            if(l==strlen(str)-CCLI_LONG_NAME_PREFIX)
                            {
                                *(int *)opt->value = atoi(argv[i++]);
                            } else {
                                int offset = str[CCLI_LONG_NAME_PREFIX+l]=='='?1:0;
                                const char *v = str + CCLI_LONG_NAME_PREFIX +l+offset;
                                *(int *)opt->value = atoi(v);
                            }
                            break;
                        case CCLI_OPT_FLOAT:
                            if(l==strlen(str)-CCLI_LONG_NAME_PREFIX)
                            {
                                *(float *) opt->value = atof(argv[i++]);
                            } else {
                                int offset = str[CCLI_LONG_NAME_PREFIX+l]=='='?1:0;
                                const char *v = str + CCLI_LONG_NAME_PREFIX +l+offset;
                                *(float *)opt->value = atof(v);
                            }
                            break;
                        case CCLI_OPT_STRING:
                            if(l==strlen(str)-CCLI_LONG_NAME_PREFIX)
                            {
                                *(const char **)opt->value = (char *)argv[++i];
                            } else {
                                int offset = str[CCLI_LONG_NAME_PREFIX+l]=='='?1:0;
                                const char *v = str + CCLI_LONG_NAME_PREFIX +l+offset;
                                *(const char **)opt->value = (char *)v;
                            }
                            break;
                        }
                    }
                }
            } else {
                if(str[1]=='h')
                {
                    ccli_help(cmd);
                    exit(0);
                }

                if(str[1]==opt->short_name)
                {
                    int l =1;
                    switch(opt->type)
                    {
                    case CCLI_OPT_BOOL:
                        *(int *)opt->value = 1;
                        break;
                    case CCLI_OPT_INT:
                        if(l==strlen(str)-CCLI_SHORT_NAME_PREFIX)
                        {
                            *(int *)opt->value = atoi(argv[i++]);
                        } else {
                            int offset = str[CCLI_SHORT_NAME_PREFIX+l]=='='?1:0;
                            const char *v = str + CCLI_SHORT_NAME_PREFIX +l+offset;
                            *(int *)opt->value = atoi(v);
                        }
                        break;
                    case CCLI_OPT_FLOAT:
                        if(l==strlen(str)-CCLI_SHORT_NAME_PREFIX)
                        {
                            *(float *) opt->value = atof(argv[i++]);
                        } else {
                            int offset = str[CCLI_SHORT_NAME_PREFIX+l]=='='?1:0;
                            const char *v = str + CCLI_SHORT_NAME_PREFIX +l+offset;
                            *(float *)opt->value = atof(v);
                        }
                        break;
                    case CCLI_OPT_STRING:
                        if(l==strlen(str)-CCLI_SHORT_NAME_PREFIX)
                        {
                            *(const char **)opt->value = (char *)argv[++i];
                        } else {
                            int offset = str[CCLI_SHORT_NAME_PREFIX+l]=='='?1:0;
                            const char *v = str + CCLI_SHORT_NAME_PREFIX +l+offset;
                            *(const char **)opt->value = (char *)v;
                        }
                        break;
                    }
                }
            }
            opt = opt->next_opt;
        }
    }
}

int
ccli_r(CCLI_CMD *root, int argc, const char **argv)
{
    if(argc==1)
    {
        return root->callback==NULL? 0 : root->callback(0,NULL);
    }
    int index = 1;
    CCLI_CMD *rcmd = ccli_r_cmd(root, &index, argc, argv);
    ccli_r_opt(rcmd, &index, argc, argv);
    return root->callback==NULL? 0 : root->callback(argc-index,argv+=index);
}
