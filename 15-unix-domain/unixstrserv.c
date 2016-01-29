#include "unp.h"

int main(int argc, char *argv[])
{
    int     listenfd, connfd;
    pid_t   childpid;
    socklen_t clilen, len;
    struct sockaddr_un cliaddr, servaddr;
    struct sockaddr_un bindaddr;
    void    sig_child(int);

    listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    unlink(UNIXSTR_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, UNIXSTR_PATH, sizeof(servaddr.sun_path) - 1);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == 1) {
        perror("bind() error");
        exit(2);
    }

    if (listen(listenfd, LISTENQ) == -1) {
        perror("listen() error");
        exit(2);
    }

    len = sizeof(bindaddr);
    getsockname(listenfd, (SA *) &bindaddr, &len);
    printf("bound to %s\n", bindaddr.sun_path);

    signal(SIGCHLD, sig_chld);

    for (;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
            if (errno == EINTR)
                continue;           /* back to for() */
            else
                err_sys("accept() error");
        }

        if ((childpid = fork()) == 0) {     /* child process */
            close(listenfd);        /* closing the listening socket */
            str_echo(connfd);       /* process the request */
            exit(0);
        }

        close(connfd);
    }
}
