#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void runOneCmd(int argc,char *argv[]){
    // for(int i=0;i<argc;i++){
    //     fprintf(1,"xargs get %s\n",argv[i]);
    // }
    exec(argv[1],argv+1);
}

int main(int argc, char *argv[]){
    //简化版UNIX的xargs程序：它从标准输入中按行读取，
    //并且为每一行执行一个命令，将行作为参数提供给命令
    char *newArgv[MAXARG];
    int newArgc=argc;
    for(int i=0;i<argc;i++){
        newArgv[i]=argv[i];
    }
    if(argc<=1){
        fprintf(2,"xargs need at least 1 args to execute\n");
        exit(1);
    }else{
        char buf[512];
        char *p=buf,*begin=buf;
        char c;
        // int nextArgIndex=argc;
        while (read(0,&c,1)>0){
            if(c=='\n'){
                if(p!=buf&&*(p-1)!='\0'){
                    *(p++)='\0';
                    newArgv[newArgc++]=begin;
                }
                int pid=fork();
                if(pid==0){
                    //子进程开始运行该命令，在runOneCmd中会执行exec命令，无需自己退出，否则就是出错了
                    runOneCmd(newArgc,newArgv);
                    exit(1);
                }else{
                    //在父进程中等待上一行执行完毕，然后重新开始读取
                    wait(0);
                    newArgc=argc;
                    p=buf;
                    begin=buf;
                }
            }else if(c==' '){//echo 1 2 3 | xargs echo 4
                if(p!=buf&&*(p-1)!=' '){
                    *(p++)='\0';
                    newArgv[newArgc++]=begin;
                    begin=p;
                }
            }else{
                (*p++)=c;
            }
        }
    }
    exit(0);
}
