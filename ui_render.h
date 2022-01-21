#ifndef      UI_RENDER_H
#define     UI_RENDER_H

#include <windows.h>
#include "union.h"
#include "core.h"

#define Wx 100
#define Hy 40

typedef struct Mdl Module;
struct Mdl{
    u16 id;
    int x,y;
    u8 status,flag,enable;
    char buf[50],*buf2,item;
    void (*Render_callback)(Module *mod);
    void (*Select_callback)(Module *mod);
    void (*Exit_callback)(Module *mod);
};

typedef struct{
    u8 Module_Length,ctrl,enable;
    char item,back_page_index;
    char title[20];
    Module module[15];
}Page;

void set_window(u8 I,u8 J);
void HideConsoleCursor();
void gotoxy(u8 x,u8 y);
void write_string(u8 I,u8 J,char *p);
void write_string2(u8 I,u8 J,char *p);

void TextView_Select_callback(Module *mod);
void TextView_Exit_callback(Module *mod);
void EditText_Select_callback(Module *mod);
void EditText_Exit_callback(Module *mod);
void PassWord_Select_callback(Module *mod);
void PassWord_Exit_callback(Module *mod);
void SelectBox_Select_callback(Module *mod);
void SelectBox_Exit_callback(Module *mod);
void ListView_Select_callback(Module *mod);
void ListView_Edit_callback(Module *mod);

void TextView_Render_callback(Module *mod);
void EditText_Render_callback(Module *mod);
void PassWord_Render_callback(Module *mod);
void SelectBox_Render_callback(Module *mod);
void ListView_Render_callback(Module *mod);

void new_Module(Module *mod,int x,int y,u16 id);
void new_Button(Module *mod,int x,int y,u16 id,char *str);
void new_TextView(Module *mod,int x,int y,u16 id,char *str);
void new_EditText(Module *mod,int x,int y,u16 id,char *str);
void new_PassWord(Module *mod,int x,int y,u16 id,char *str);
void new_SelectBox(Module *mod,int x,int y,u16 id,char *str,int index);
void new_ListView(Module *mod,int x,int y,u16 id,char *str);


void new_page(Page *page,u8 Module_Length,char back_page_index,char *str);

void render();
void render2();

void page_init();
void page_manager(char ctrl);

void ui_render_init();

#endif