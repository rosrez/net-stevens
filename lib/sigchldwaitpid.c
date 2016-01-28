#include "unp.h"

void sig_chld(int signo)
{
    pid_t   pid;
    int     stat;
    int     saved_errno;

    saved_errno = errno;

    while ((pid == waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);

    errno = saved_errno;

    return;
}
