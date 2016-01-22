#include "unp.h"

void str_echo(int sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];
    pid_t   pid;

    pid = getpid();
    
    printf("Server process %d started servicing client\n", pid);
    for (;;) {
        if ((n = read(sockfd, buf, MAXLINE)) <= 0) {
            if (n == -1)
                perror("str_echo: read() failed");
            return;     /* connection closed by peer */
        }

        buf[n] = '\0';
        printf("Server %d read %ld bytes: %s\n", pid, (long) n, buf);

        writen(sockfd, buf, n);
    }
}
