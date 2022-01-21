#include "core.h"

char buffer[100];
u32 lb=0;
u8 exit_flag=1;

void set_flag(u8 cmd){
    exit_flag=cmd;
}
u8 get_flag(){
    return exit_flag;
}
void core_init(){
    
}
void run_core(char cmd){
    page_manager(cmd);
}
