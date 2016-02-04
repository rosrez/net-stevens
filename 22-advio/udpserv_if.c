#include "unpifi.h"

/* 
 * Stevens, 22.13.
 * A UDP server that binds with all available interface addresses.
 */

void mydg_echo(int, SA *, socklen_t, SA *);

int main(int argc, char *argv[])
{
    int     sockfd;
    const int on = 1;
    pid_t   pid;
    struct ifi_info *ifi, *ifihead;
    struct sockaddr_in *sa, cliaddr, wildaddr;

    for (ifihead = ifi = get_ifi_info(AF_INET, 1);
            ifi != NULL; ifi = ifi->ifi_next) {

        /* bind a unicast address */
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        /* 
         * Berkley implementations don't require SO_REUSEADDR in this case:
         * i.e. when the new binding address is not INADDR_ANY and is
         * different from all other addresses already bound to this port.
         */
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        sa = (struct sockaddr_in *) ifi->ifi_addr;
        sa->sin_family = AF_INET;
        sa->sin_port = htons(SERV_PORT);
        if (bind(sockfd, (SA *) sa, sizeof(*sa)) == -1) {
            printf("bind failed for address %s\n", sock_ntop((SA *) sa, sizeof(*sa)));
            perror("");
        }

        printf("bound %s\n", sock_ntop((SA *) sa, sizeof(*sa)));

        if ((pid = fork()) == 0) {  /* child process */
            mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
            exit(0);
        }

        if (ifi->ifi_flags & IFF_BROADCAST) {
            /* try to bind a broadcast address */
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

            sa = (struct sockaddr_in *) ifi->ifi_brdaddr;
            sa->sin_family = AF_INET;
            sa->sin_port = htons(SERV_PORT);
            if (bind(sockfd, (SA *) sa, sizeof(*sa)) < 0) {
                if (errno == EADDRINUSE) {
                    printf("address in use: %s\n", sock_ntop((SA *) sa, sizeof(*sa)));
                } else {
                    err_sys("bind error for %s", sock_ntop((SA *) sa, sizeof(*sa)));
                }
            }
            printf("bound %s\n", sock_ntop((SA *) sa, sizeof(*sa)));

            if ((pid = fork()) == 0) {      /* child process */
                mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
                exit(0);
            }
        }
    }

    /* bind INADDR_ANY */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&wildaddr, sizeof(wildaddr));
    wildaddr.sin_family = AF_INET;
    wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    wildaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (SA *) &wildaddr, sizeof(wildaddr)) == -1)
        perror("could not bind to INADDR_ANY");

    printf("bound to %s\n", sock_ntop((SA *) &wildaddr, sizeof(wildaddr)));

    if ((pid = fork()) == 0) {      /* child process */
        mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
        exit(0);
    }

    free_ifi_info(ifihead);
    exit(0);   
}

void mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, SA *myaddr)
{
    int     n;
    char    mesg[MAXLINE];
    socklen_t len;

    for (;;) {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        printf("child %d, datagram from %s", getpid(), sock_ntop(pcliaddr, len));
        printf(", to %s\n", sock_ntop(myaddr, clilen));

        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}
