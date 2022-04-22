#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent2child[2],child2parent[2];
    char buf[1];
    pipe(parent2child);
    pipe(child2parent);
    // fprintf(1,"%d,%d\n",pipeEnd[0],pipeEnd[1]);
    int pid=fork();
    int exitStatus=0;
    if(pid<0){
        fprintf(2, "fork() error!\n");
        close(child2parent[0]);
        close(child2parent[1]);
        close(parent2child[0]);
        close(parent2child[1]);
        exit(1);
    }else if(pid==0){
        close(parent2child[1]);
        close(child2parent[0]);
        if(read(parent2child[0],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "child read error!\n");
            exitStatus=1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }
        if(write(child2parent[1],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "child write error!\n");
            exitStatus=1;
        }
        close(parent2child[0]);
        close(child2parent[1]);
    }else{
        close(parent2child[0]);
        close(child2parent[1]);
        if(write(parent2child[1],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "parent write error!\n");
            exitStatus=1;
        }
        if(read(child2parent[0],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "parent read error!\n");
            exitStatus=1;
        }else{
            fprintf(1,"%d: received pong\n",getpid());
        }
        close(parent2child[1]);
        close(child2parent[0]);
    }
    exit(exitStatus);
}
