#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    struct  sockaddr_un servaddr, cliaddr;

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    unlink(UNIXDG_PATH);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);

    if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("bind() error");
        exit(2);
    }

    printf("bound to %s\n", servaddr.sun_path);

    dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
