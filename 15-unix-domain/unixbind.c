#include "unp.h"

int main(int argc, char *argv[])
{
    int sockfd;
    socklen_t len;
    struct sockaddr_un addr1, addr2;

    if (argc != 2)
        err_quit("usage: unixbind <pathname>");

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    /* 
     * bind() will fail if the path to UNIX domain socket already exists:
     * we therefore have to remove the file prior to calling bind().
     */
    unlink(argv[1]);        /* ignoring possible errors */

    bzero(&addr1, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    /* 
     * sizeof(addr1.sun_path) - 1 to ensure 1 byte is left for
     * the terminating null character. 
     */
    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
    bind(sockfd, (SA *) &addr1, SUN_LEN(&addr1));

    len = sizeof(addr2);
    getsockname(sockfd, (SA *) &addr2, &len);
    printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);

    exit(0);
}
