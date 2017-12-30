#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>

#if __WORDSIZE == 64
    #define SYSCALL(reg) reg.orig_rax
    #define FD(reg) reg.rdi
    #define BUF(reg) reg.rsi
    #define COUNT(reg) reg.rdx
    #define INFERIOR "inferior64.bin"
#else
    #define SYSCALL(reg) reg.orig_eax
    #define FD(reg) reg.ebx
    #define BUF(reg) reg.ecx
    #define COUNT(reg) reg.edx
    #define INFERIOR "inferior32.bin"
#endif

static void list_write_calls(pid_t child)
{
    struct user_regs_struct regs;
    int	status;
    long last_call;
    while(waitpid(child, &status, 0) && !WIFEXITED(status))
    {
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        last_call = SYSCALL(regs);
        if(SYS_write == last_call)
        {
             printf("****** pid %d - syscall write %p --------\n", child, last_call);
             printf("\tfd %u; buf %x; count %zu\n", FD(regs), BUF(regs),COUNT(regs));

        }
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
}

static void setup_inferior(const char *path, char argv[])
{
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execl(path, argv, 0);
}


void dbg_inferior_exec(const char *path, char argv[])
{
    pid_t child;
    do
    {
        child = fork();
        switch (child)
        {
            case 0:   // inferior
                setup_inferior(path, argv);
                break;
            case -1:  // error
                break;
            default:  // debugger
                list_write_calls(child);
                break;
        }
    }
    while (child == -1 && errno == EAGAIN);
}

/* gcc -m32/-m64 -o ptrace_write ptrace_write.c*/
int main()
{
    printf("****** TRACING %s\n", INFERIOR);
    char argv[1] = { 0 };
    dbg_inferior_exec(INFERIOR, argv);
    return 0;
} 
