
#include <stdio.h>
#include "ccli.h"


void 
ccli_run(struct ccli_command *cmd, int argc, const char **argv){
    printf("参数数量：%d\n", argc);
    for(int i=0;i<argc;i++){
            printf("%s\n", argv[i]);
    }
    
    int len = sizeof(cmd->sub_commands);
    printf("cmds：%s\n", cmd->options->long_name);
    
    /*
    if( len>0 ){
        for(int i=0;i<len;i++){
                printf("Command：%s\n", cmd->sub_commands->name);
        }
    }
    */
    
}
