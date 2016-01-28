#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    struct  sockaddr_un cliaddr, servaddr;

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));

    if (bind(sockfd, (SA *) &cliaddr, sizeof(cliaddr)) == -1) {
        perror("bind() error");
        exit(2);
    }

    printf("bound client to %s\n", cliaddr.sun_path);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);

    printf("sending to %s\n", servaddr.sun_path);

    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    exit(0);
}
