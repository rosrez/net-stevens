#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    struct  sockaddr_un servaddr;

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, UNIXSTR_PATH, sizeof(servaddr.sun_path) - 1);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("connect() error");
        exit(2);
    }

    printf("connected to %s\n", servaddr.sun_path);

    str_cli(stdin, sockfd);     /* does all the work */

    exit (0);
}
