#include "unp.h"
#include "dgcli.h"

/* Stevens, 20.1
 * UDP broadcast client: broadcasts a datagram and collects replies */

static void recvfrom_alarm(int);

void dg_cli_bcast(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n;
    const int on = 1;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr = malloc(servlen);

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1) 
        perror("setsockopt() error");

    signal(SIGALRM, recvfrom_alarm);

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        alarm(5);

        for ( ; ; ) {
            len = servlen;
            n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
            if (n < 0) {
                if (errno == EINTR)
                    break;          /* finish waiting for responses */
                else
                    err_sys("recvfrom() error");
            } else {
                recvline[n] = 0;    /* terminating null */
                printf("from %s: %s", sock_ntop_host(preply_addr, len), recvline);
            }
        }
    }
    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    return;     /* will interrupt recvfrom() */
}
