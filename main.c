#include "main.h"

void sys_init(){
    ui_render_init();
    core_init();
}

void main(){
    sys_init();
    while(get_flag()){
        run_core(getch());
    }
}