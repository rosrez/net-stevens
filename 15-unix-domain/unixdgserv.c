#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    socklen_t len;
    struct  sockaddr_un servaddr, cliaddr;
    struct  sockaddr_un bindaddr;

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    unlink(UNIXDG_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, UNIXDG_PATH, sizeof(servaddr.sun_path) - 1);

    if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("bind() error");
        exit(2);
    }

    len = sizeof(bindaddr);
    getsockname(sockfd, (SA *) &bindaddr, &len);
    printf("bound server to %s\n", bindaddr.sun_path);

    /* never returns */
    dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));

    /* to keep the compiler happy */
    return 0;
}
