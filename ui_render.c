#include "ui_render.h"

char test[]="Hello world!!!";

Page page[3];
u16 page_index=0;
u16 position=0;

void set_window(u8 I,u8 J){
    char str[20];
    sprintf(str,"mode con cols=%d lines=%d",I,J);
    system(str);
}
void HideConsoleCursor(){
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void gotoxy(u8 x,u8 y){
	COORD p;
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
    p.X=x;p.Y=y;
	SetConsoleCursorPosition(handle,p);
}
void write_string(u8 I,u8 J,char *p){
    gotoxy(I,J);
    printf("%s",p);
}
void write_string2(u8 I,u8 J,char *p){
    gotoxy(I,J);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x70);
    printf("%s",p);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
}

void TextView_Select_callback(Module *mod){
    mod->status=!mod->status;
    if(mod->flag&&mod->status){
        switch(mod->id){
            case 5:
                page_index=1;
                render2();
                break;
            case 15:
                page_index=0;
                render2();
                break;
            case 8:
                set_flag(0);
                break;
        }
        render2();
        mod->flag=0;
    }
}
void TextView_Exit_callback(Module *mod){}
void EditText_Select_callback(Module *mod){
    mod->status=1;
    mod->buf[0]='\0';
    render2();
    if(mod->flag&&mod->status){
        switch(mod->id){
            case 2:
            case 13:
                SetConsoleTitle("输入状态");
                gotoxy(mod->x,mod->y);
                gets(mod->buf);
                break;
            case 4:
            case 11:
                SetConsoleTitle("密码状态");
                gotoxy(mod->x,mod->y);
                gets(mod->buf);
                break;
        }
        mod->flag=0;
        render2();
    }
}
void EditText_Exit_callback(Module *mod){}
void PassWord_Select_callback(Module *mod){
    EditText_Select_callback(mod);
}
void PassWord_Exit_callback(Module *mod){}
void SelectBox_Select_callback(Module *mod){
    mod->status=1;
    if(mod->flag&&mod->status){
        switch(mod->id){
            case 16:
                if(strlen(mod->buf2)==0){
                    mod->flag=0;
                    return;
                }
                match(mod->buf2,'|');
                break;
        }
        render2();
        while(mod->flag){
            switch(getch()){
                case 27:
                case 13:
                    mod->flag=0;
                    render2();
                    return;
                case 9:
                    mod->item=(++mod->item)%get_Len();
                    render2();
                    break;
                case 8:
                    mod->item=(--mod->item)%get_Len();
                    render2();
                    break;
            }
        }
        mod->flag=0;
    }
}
void SelectBox_Exit_callback(Module *mod){}
void ListView_Select_callback(Module *mod){}
void ListView_Edit_callback(Module *mod){}

void TextView_Render_callback(Module *mod){
    char str[100];
    if(mod->status){
        if(mod->flag){
            write_string2(mod->x,mod->y,mod->buf);
        }else{
            sprintf(str,"\b \b[%s]",mod->buf);
            write_string(mod->x,mod->y,str);
        }
    }else{
        write_string(mod->x,mod->y,mod->buf);
    }
}
void EditText_Render_callback(Module *mod){
    if(mod->status){
        if(mod->flag){
            write_string2(mod->x,mod->y,"\b \b                          ");
        }else{
            write_string(mod->x,mod->y,"\b \b[                        ]");
        }
        write_string(mod->x,mod->y+1,"\b \b==========================");
    }else{
        write_string(mod->x,mod->y,"\b \b                          ");
        write_string(mod->x,mod->y+1,"\b \b--------------------------");
    }
    write_string(mod->x,mod->y,mod->buf);
}
void PassWord_Render_callback(Module *mod){
    char s[100];
    u8 i=0;
    EditText_Render_callback(mod);
    for(i=0;i<strlen(mod->buf);i++){
        s[i]='*';
    }
    s[i]='\0';
    write_string(mod->x,mod->y,s);
}
void SelectBox_Render_callback(Module *mod){
    if(strlen(mod->buf2)==0||mod->item<0){
        mod->item=-1;
        copy2(mod->buf,"");
    }else{
        match(mod->buf2,'|');
        if(mod->item<get_Len()){
            get_result(mod->buf,mod->item);
        }else{
            mod->item=-1;
            copy2(mod->buf,"");
        }
    }
    TextView_Render_callback(mod);
}
void ListView_Render_callback(Module *mod){}

void new_Module(Module *mod,int x,int y,u16 id){
    mod->x=x;
    mod->y=y;
    mod->id=id;
    mod->flag=0;
}
void new_TextView(Module *mod,int x,int y,u16 id,char *str){
    new_Module(mod,x,y,id);
    copy2(mod->buf,str);
    mod->status=0;mod->enable=0;
    mod->Render_callback=TextView_Render_callback;
    mod->Select_callback=TextView_Select_callback;
    mod->Exit_callback=TextView_Exit_callback;
}
void new_Button(Module *mod,int x,int y,u16 id,char *str){
    new_TextView(mod,x,y,id,str);
    mod->enable=1;
}
void new_EditText(Module *mod,int x,int y,u16 id,char *str){
    new_Module(mod,x,y,id);
    copy2(mod->buf,str);
    mod->status=0;mod->enable=1;
    mod->Render_callback=EditText_Render_callback;
    mod->Select_callback=EditText_Select_callback;
    mod->Exit_callback=EditText_Exit_callback;
}
void new_PassWord(Module *mod,int x,int y,u16 id,char *str){
    new_Module(mod,x,y,id);
    copy2(mod->buf,str);
    mod->status=0;mod->enable=1;
    mod->Render_callback=PassWord_Render_callback;
    mod->Select_callback=PassWord_Select_callback;
    mod->Exit_callback=PassWord_Exit_callback;    
}
void new_SelectBox(Module *mod,int x,int y,u16 id,char *str,int index){
    new_Module(mod,x,y,id);
    mod->buf2=str;
    if(strlen(str)==0||index<0){
        mod->item=-1;
        copy2(mod->buf,"");
    }else{
        match(str,'|');
        if(index<get_Len()){
            mod->item=index;
            get_result(mod->buf,index);
        }else{
            mod->item=-1;
            copy2(mod->buf,"");
        }
    }
    mod->status=0;mod->enable=1;mod->id=index;
    mod->Render_callback=SelectBox_Render_callback;
    mod->Select_callback=SelectBox_Select_callback;
    mod->Exit_callback=SelectBox_Exit_callback;
}
void new_ListView(Module *mod,int x,int y,u16 id,char *str){
    new_Module(mod,x,y,id);
    copy2(mod->buf,str);
    mod->status=0;mod->enable=0;
    mod->Render_callback=ListView_Render_callback;
    mod->Select_callback=ListView_Select_callback;
    mod->Exit_callback=ListView_Edit_callback;
}

void new_page(Page *page,u8 Module_Length,char back_page_index,char *str){
    page->ctrl=0;
    page->item=-1;
    page->Module_Length=Module_Length;
    page->back_page_index=back_page_index;
    page->enable=1;
    sprintf(page->title,"%s",str);
}

void render(){
    u16 i=0;
    if(page_index>=0){
        SetConsoleTitle(page[page_index].title);
        for(i=0;i<page[page_index].Module_Length;i++){
            Module *p=&page[page_index].module[i];
            position=(position<(p->y))?(p->y):position;
            page[page_index].module[i].Render_callback(p);
        }
    }
    gotoxy(0,0);
}
void render2(){
    system("cls");
    position=0;
    render();
    HideConsoleCursor();
}
void page_manager(char ctrl){
    if(ctrl==27&&(page_index==-1||page[page_index].back_page_index==-1)){set_flag(0);}
    switch(ctrl){
        case 27:
            SetConsoleTitle("ESC回退");
            if(page[page_index].back_page_index!=-1){
                    page_index=page[page_index].back_page_index;
                    render2();
            }else{
                set_flag(0);
            }
            break;
        case 9:
            SetConsoleTitle("切换状态");
            if(page_index!=-1&&page[page_index].enable){
                if(page[page_index].item){
                    page[page_index].module[page[page_index].item-1].status=0;
                }
                do{
                    page[page_index].item=(++page[page_index].item)%(page[page_index].Module_Length+1);
                    if(page[page_index].item==0){
                        break;
                    }
                }while(page[page_index].module[page[page_index].item-1].enable==0);
                if(page[page_index].item){
                    page[page_index].module[page[page_index].item-1].status=1;
                }
                render2();
            }
            break;
        case 8:
            SetConsoleTitle("回退状态");
            if(page_index!=-1&&page[page_index].enable){
                if(page[page_index].item){
                    page[page_index].module[page[page_index].item-1].status=0;
                }
                do{
                    page[page_index].item=(--page[page_index].item)%(page[page_index].Module_Length+1);
                    if(page[page_index].item==0){
                        break;
                    }
                }while(page[page_index].module[page[page_index].item-1].enable==0);
                if(page[page_index].item){
                    page[page_index].module[page[page_index].item-1].status=1;
                }
                render2();
            }
            break;
        case 13:
            if(page[page_index].item){
                Module *md=&page[page_index].module[page[page_index].item-1];
                md->flag=1;
                md->Select_callback(md);
                md->Exit_callback(md);
            }
            break;  
    }
}

void page_init(){
    page_index=0;

    new_page(&page[0],9,-1,"UI_SYS系统----登录界面");
    new_TextView(&page[0].module[0],38,5,0,"欢迎来到我的UI系统");
    
    new_TextView(&page[0].module[1],33,11,1,"账号: ");
    new_EditText(&page[0].module[2],42,11,2,"");
    
    new_TextView(&page[0].module[3],33,13,3,"密码: ");
    new_PassWord(&page[0].module[4],42,13,4,"");
    
    new_Button(&page[0].module[5],33,17,5,"注册账号");
    new_Button(&page[0].module[6],53,17,6,"忘记密码");
    new_Button(&page[0].module[7],43,19,7,"登陆系统");
    new_Button(&page[0].module[8],43,23,8,"退出系统");

    new_page(&page[1],9,0,"UI_SYS系统----注册界面");
    new_TextView(&page[1].module[0],30,5,9,"欢迎来到我的第二个页面,注册新用户");
    
    new_TextView(&page[1].module[1],33,11,10,"账号: ");
    new_EditText(&page[1].module[2],42,11,11,"admin");
    
    new_TextView(&page[1].module[3],33,13,12,"密码: ");
    new_PassWord(&page[1].module[4],42,13,13,"000000");
    
    new_TextView(&page[1].module[5],33,17,17,"类型: ");
    new_SelectBox(&page[1].module[6],42,17,16,"管理员|学生|教师",1);

    new_Button(&page[1].module[7],43,21,14,"注册用户");
    new_Button(&page[1].module[8],43,25,15,"返回登陆");
}
void ui_render_init(){
    SetConsoleTitle("");
    set_window(Wx,Hy);
    HideConsoleCursor();
    page_init();
    render2();
}