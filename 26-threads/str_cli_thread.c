#include "unpthread.h"

static void *copyto(void *);

static int sockfd;              /* global for both threads to access */
static FILE *fp;

/*
 * str_cli_thread() creates a worker thread to handle
 * sending of data to the server and then enters the receiving
 * loop to read server responses (until the server closes the connection).
 *
 * TYPICAL SCENARIOS:
 * 1. The sending thread has processed all data from the file and
 * terminated. The receiving (main) thread might still be receiving server
 * responses.
 *
 * 2. The server terminates prematurely: i.e. before reading all client data. 
 * This will cause str_cli_thread() to return to main() and main will exit,
 * terminating the sending thread, which is what we want.
 */
void str_cli_thread(FILE *fp_arg, int sockfd_arg)
{
    char    recvline[MAXLINE];
    pthread_t tid;

    sockfd = sockfd_arg;        /* copy arguments to externals */
    fp = fp_arg;

    /* create the sending thread */
    Pthread_create(&tid, NULL, copyto, NULL);

    /* copy received data from socket to STDOUT */
    while (readline(sockfd, recvline, MAXLINE) > 0)
        fputs(recvline, stdout);
}

/*
 * This is the thread function for the sending thread. 
 * It continuously copies data from the file pointer (which may
 * refer to STDIN our an arbitrary file) to the socket.
 * Once we're done with copying, the sending thread sends a FIN
 * to the server to indicate that it has completed sending.
 */
static void *copyto(void *arg)
{
    char    sendline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL)
        writen(sockfd, sendline, strlen(sendline));

    shutdown(sockfd, SHUT_WR);      /* EOF on stdin, send FIN */

    /* thread terminates (when EOF on stdin) */
    return NULL;
}
