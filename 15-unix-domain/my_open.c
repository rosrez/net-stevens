#include "unp.h"

int my_open(const char *pathname, int mode)
{
    int     fd, sockfd[2], status;
    pid_t   childpid;
    char    c, argsockfd[10], argmode[10];

    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) == -1) {
        perror("socketpair() error");
        exit(2);
    }

    if ((childpid = fork()) == 0) {     /* child process */
        close(sockfd[0]);
        snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
        snprintf(argmode, sizeof(argmode), "%d", mode);
        execl("./openfile", "openfile", argsockfd, pathname, argmode,
                (char *) NULL);
        err_sys("execl() error");
    }

    /* close the socket end the parent doesn't use */
    close(sockfd[1]);

    /* parent process - wait for the child to terminate */
    waitpid(childpid, &status, 0);
    if (WIFEXITED(status) == 0)
        err_quit("child did not terminate");
    if ((status = WEXITSTATUS(status)) == 0) {
        read_fd(sockfd[0], &c, 1, &fd);
    } else {
        /* set errno to child's exit code */
        errno = status;

        fd = -1;
    }

    close(sockfd[0]);
    return fd;
}
