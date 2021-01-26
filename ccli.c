
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
    cmd->prev_cmd = NULL;
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
                root->last_sub_cmd->prev_cmd = root->first_sub_cmd;
            }
        }else{
            root->last_sub_cmd->next_cmd = sub_cmd;
            sub_cmd->prev_cmd = root->last_sub_cmd;
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
    opt->prev_opt = NULL;
    opt->next_opt = NULL;
    opt->value = value;
    opt->help = help;
    return opt;
}

void
set_opt(CCLI_CMD *cmd, enum ccli_option_type type, char short_name, const char *long_name, void *value, const char *help)
{
    CCLI_OPT *opt = create_opt(type, short_name, long_name, value, help);
    if(cmd->first_opt==NULL){
        cmd->first_opt = opt;
    }
    
    if(cmd->last_opt==NULL)
    {
        cmd->last_opt = opt;
        if(cmd->first_opt!=opt)
        {
            cmd->first_opt->next_opt = opt;
            opt->prev_opt = cmd->first_opt;
        }
    }else{
        cmd->last_opt->next_opt = opt;
        opt->prev_opt = cmd->last_opt;
        cmd->last_opt = opt;
    }
}

#define PRINTF_OPT(...) printf("  -%c, --%s %s  %s\n",__VA_ARGS__);

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
        curCmd = cmd->first_sub_cmd;
        while(curCmd){
              printf("  %s  %s\n",curCmd->name, curCmd->description);
              curCmd = curCmd->next_cmd;
        }
    }
    
    printf("\nOptions:\n");
    if(cmd->first_opt!=NULL)
    {
        CCLI_OPT* curOpt = cmd->first_opt;
        while(curOpt){
            switch(curOpt->type)
            {
                case CCLI_OPT_INT:
                    PRINTF_OPT(curOpt->short_name, curOpt->long_name,"int", curOpt->help);
                    break;
                case CCLI_OPT_FLOAT:
                    PRINTF_OPT(curOpt->short_name, curOpt->long_name,"float", curOpt->help);
                    break;
                case CCLI_OPT_STRING:
                    PRINTF_OPT(curOpt->short_name, curOpt->long_name,"string", curOpt->help);
                    break;
                default:
                    PRINTF_OPT(curOpt->short_name, curOpt->long_name,"", curOpt->help);
            }
            curOpt = curOpt->next_opt;
        }
    }
    printf("  -h, --help  print help message.\n");
    
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

void
ccli_r_opt(CCLI_CMD *cmd, int *index, int argc, const char **argv)
{
    for(int i=*index;i<argc;i++)
    {
        if(i>=argc){
            return;
        }
        const char *str = argv[i];
        if(str[0] != '-' || strlen(str)<2)
        {
            return;
        }
        CCLI_OPT *opt = cmd->first_opt;
        
        while(opt)
        {
            if(str[1]=='-')
            {
                if(strlen(str)>=(strlen(opt->long_name)+2))
                {
                    if(strcoll(str, opt->long_name)==-52)
                    {
                        
                    }
                }
            }else{
                if(str[1]==opt->short_name)
                {
                    
                }
            }
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
    
    for(int i=0;i<argc;i++)
    {
            printf("%s\n", argv[i]);
    }
    //CCLI_CMD *cmd = root;//->first_sub_cmd;
    //ccli_help(cmd);
    return 0;
}
