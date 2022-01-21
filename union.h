#ifndef     UNION_H
#define     UNION_H

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char result[100][100];
static u32 Len=0;

void copy(char *p,char *q,u16 l);
void copy2(char *p,char *q);
u32 get_Len();
u8 get_result(char *p,u32 n);
void match(char *p,char q);
void match_from_item(u32 n,char q);
u8 check(char q);

void show();

#endif