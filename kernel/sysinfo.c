#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sysinfo.h"
#include "proc.h"

uint64
sys_info(void)
{
    struct sysinfo info; // 创建一个info指针
    uint64 st;           // user pointer to struct sysinfo

    //调用kalloc.c中的函数获取空闲内存量
    info.freemem = get_free_mem();

    //调用proc.c中的函数获取进程数
    info.nproc = get_proc_mem();

    if (argaddr(0, &st) < 0)
        return -1;
    struct proc *p = myproc();
    if (copyout(p->pagetable, st, (char *)&info, sizeof(info)) < 0)
        return -1;
    return 0;
}