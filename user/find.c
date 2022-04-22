#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void dfs(char *path,char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        fprintf(2,"find: %s is a file,which should be a path\n",path);
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        // buf=""
        strcpy(buf, path);// buf="path"
        p = buf+strlen(buf);
        *p++ = '/';// buf="path/"
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if(st.type==T_FILE){
                // printf("buf=%s filename=%s\n",buf,p);
                if(strcmp(p,name)==0){
                    printf("%s\n",buf);
                }
            }else if(st.type==T_DIR){
                // printf("buf=%s dirname=%s\n",buf,p);
                if(strcmp(p,".")!=0&&strcmp(p,"..")!=0){
                    dfs(buf,name);
                }
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]){
    if(argc!=3){
        fprintf(2, "give %d args,but need 2\n", argc-1);
    }else{
        dfs(argv[1],argv[2]);
    }
    exit(0);
}
