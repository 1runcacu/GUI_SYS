#include "union.h"

void copy(char *p,char *q,u16 l){
    strncpy(p,q,l);
    p[l]='\0';
}
void copy2(char *p,char *q){
    copy(p,q,strlen(q));
}
u32 get_Len(){
    return Len;
}
u8 get_result(char *p,u32 n){
    if(n<Len){
        copy2(p,result[n]);
        return 1;
    }
    return 0;
}
void match(char *p,char q){
    u32 i=0,j=1,Lp=strlen(p);
    Len=0;
    while(i<Lp && Len<300){
        while(p[j]!=q && j<Lp){
            if(p[j]=='\n'){
                break;
            }
            j++;
        }
        copy(result[Len++],&p[i],j-i);
        i=++j;
    }
}
void match_from_item(u32 n,char q){
    u32 i=0,j=1;
    char p[300];
    sprintf(p,"%s",result[n]);
    match(p,q);
}

u8 check(char q){
    if(q>31&&q<128){
        if(q=='\n'){
            return 1;
        }else if(q==' '){
            return 2;
        }else if(q>='0'&&q<='9'){
            return 3;
        }else if(q>='A'&&q<='Z'){
            return 4;
        }else if(q>='a'&&q<='z'){
            return 5;
        }else{
            return 6;
        }
    }else{
        return 0;
    }
}

void show(){
    u32 i=0;
    for(i=0;i<Len;i++){
        printf("%s\n",result[i]);
    }
}