/*
 * Stevens, Ch. 16.2
 * A version of str_cli that forks a child process to do socket/stdin/stdout I/O:
 * parent is used to read from stdin and write to socket;
 * child is used to read responses from socket and write them to stdout.
 *
 * This has just a small performance overhead compared to the nonblocking version
 * of the function str_cli_nonb(), which never blocks in read()/write()
 * calls. The support for nonblocking makes that function significantly more complex.
 */

#include "unp.h"

void str_cli_fork(FILE *fp, int sockfd)
{
    pid_t   pid;
    char    sendline[MAXLINE], recvline[MAXLINE];

    if ((pid = fork()) == 0) {  /* child process: server -> stdout */
        while (readline(sockfd, recvline, MAXLINE) > 0)
            fputs(recvline, stdout);

        /*  if parent is still running, terminate it */
        kill(getppid(), SIGTERM);
        
        exit(0);
    }

    /* parent */
    while (fgets(sendline, MAXLINE, fp) != NULL)
        writen(sockfd, sendline, strlen(sendline));
    shutdown(sockfd, SHUT_WR);  /* end of file on STDINT, send FIN */
    /* wait for signal */
    pause();
    return;
}
