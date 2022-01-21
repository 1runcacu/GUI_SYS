#ifndef     CORE_H
#define     CORE_H

#include "union.h"
#include <windows.h>
#include <conio.h>
#include "ui_render.h"

void set_flag(u8 cmd);
u8 get_flag();
void core_init();
void run_core(char cmd);

#endif