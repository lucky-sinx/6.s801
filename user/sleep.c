#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc==1){
        fprintf(2,"Sleep: no time argument!\n");
        exit(1);
    }else {
        int sleeptime=atoi(argv[1]);
        sleep(sleeptime);
        exit(0);
    }
}
