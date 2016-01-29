#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    socklen_t len;
    struct  sockaddr_un cliaddr, servaddr;
    struct  sockaddr_un bindaddr;

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));

    if (bind(sockfd, (SA *) &cliaddr, sizeof(cliaddr)) == -1) {
        perror("bind() error");
        exit(2);
    }

    len = sizeof(bindaddr);
    getsockname(sockfd, (SA *) &bindaddr, &len);
    printf("bound client to %s\n", bindaddr.sun_path);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, UNIXDG_PATH, sizeof(servaddr.sun_path) - 1);

    /* 
     * We could call connect() to permanently bind our client 
     * to the server address, which suits *this* client perfectly.
     *
     * We would have to rewrite the dg_cli() function to either
     * supply NULL/zero arguments for the target address and 
     * address length, since this is expected for connected sockets.
     *
     * Not calling connect() allows us to send data to multiple destinations
     * using the same datagram socket, just as with UDP.
     */

#if 0
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("connect() error");
        exit(2);
    }
#endif

    printf("sending to %s\n", servaddr.sun_path);

    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    exit(0);
}
