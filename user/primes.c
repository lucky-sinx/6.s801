#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void rightProcess(int* pipeEnd){
    close(pipeEnd[1]);
    int t;
    int firstPrime=0;
    int hasRight=0;
    int rightPipe[2];
    while (read(pipeEnd[0],&t,sizeof(t))>0){
        if(firstPrime==0){
            firstPrime=t;
            printf("prime %d\n",firstPrime);
        }else{
            if(t%firstPrime!=0){
                if(hasRight==0){
                    //还没有右子进程,创建一个右子进程
                    hasRight=1;
                    pipe(rightPipe);
                    int pid=fork();
                    if(pid==0){
                        rightProcess(rightPipe);
                        exit(0);
                    }else{
                        close(rightPipe[0]);
                    }
                }
                write(rightPipe[1],&t,sizeof(t));//输出给右子进程
            }
        }
    }
    if(hasRight){
        close(rightPipe[1]);
        wait(0);
    }
    close(pipeEnd[0]);
}

int main(int argc, char *argv[])
{
    //第一个进程，负责输出到下一进程
    int pipeEnd[2];
    const int N=35;
    pipe(pipeEnd);
    int pid=fork();
    if(pid<0){
        fprintf(2, "fork() error!\n");
        close(pipeEnd[0]);
        close(pipeEnd[1]);
        exit(1);
    }else if(pid==0){
        //右邻居，负责接受这里的输出
        rightProcess(pipeEnd);
        exit(0);
    }else{
        //将2-35输出
        close(pipeEnd[0]);
        for(int i=2;i<=N;i++){
            write(pipeEnd[1],&i,sizeof(i));
        }
        close(pipeEnd[1]);
        wait(0);
        exit(0);
    }
}
