#include "unp.h"

void str_cli_nonb(FILE *fp, int sockfd)
{
    int     maxfdp1, val, stdineof;
    ssize_t n, nwritten;
    fd_set  rset, wset;
    char    to[MAXLINE], fr[MAXLINE];
    char    *toiptr, *tooptr, *friptr, *froptr;

    val = fcntl(sockfd, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDOUT_FILENO, F_GETFL, 0);
    fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    /*
     * toiptr - pointer to store the next character read from STDIN
     * tooptr - pointer to copy already read characters to socket
     *
     * This holds for toiptr, tooptr at all times
     * tooptr <= toiptr,
     * 
     * i.e reading point from STDIN is equal to/ahead of writing point
     * to the socket.
     *
     * friptr - pointer to store the next character read from socket
     * froptr - pointer to copy already received characters to STDOUT
     *
     * This holds for friptr, froptr at all times
     * frooptr <= friptr,
     * 
     * i.e reading point from STDIN is equal to/ahead of writing point
     * to the socket.
     */
    toiptr = tooptr = to;
    friptr = froptr = fr;
    stdineof = 0;

    maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&rset);

        /* no EOF encountered on STDIN and there is input buffer
         * space to read from STDIN: include the STDIN descriptor
         * in the read set */
        if (!stdineof && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO, &rset);

        /* there's buffer space to read server responses from socket:
         * include the socket file descriptor in the read set */
        if (friptr < &fr[MAXLINE])
            FD_SET(sockfd, &rset);

        /* there's read but unsent data:
         * include the socket file descriptor in the write set */
        if (tooptr != toiptr)
            FD_SET(sockfd, &wset);

        /* there's received data that is not output to STDOUT yet:
         * include STDOUT in the write set */
        if (froptr != friptr)
            FD_SET(STDOUT_FILENO, &wset);

        if (select(maxfdp1, &rset, &wset, NULL, NULL) == -1) {
            if (errno == EINTR)
                continue;

            perror("select() error");
            exit(2);
        }

        /* STDIN input */
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            if ((n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
                if (errno != EWOULDBLOCK)
                    err_sys("read() error on stdin");

            /* encountered EOF on STDIN */
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on stdin\n", gf_time());
                /* set the flag not to check for STDIN input anymore */
                stdineof = 1;
                /* if done sending data read from stdin up to now,
                 * sent FIN to server, i.e. close our writing end of connection */
                if (tooptr == toiptr)
                    shutdown(sockfd, SHUT_WR);

            /* successfully read data from stdin */
            } else {
                fprintf(stderr, "%s: read %ld bytes from stdin\n", gf_time(), (long) n);
                /* advance the 'read from stdin' pointer */
                toiptr += n;
            }
        }

        /* socket input */
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
                if (errno != EWOULDBLOCK)
                    err_sys("read error on socket");

            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on socket\n", gf_time());
                /* server sends FIN; if also finished reading our STDIN, terminate normally */
                if (stdineof)
                    return;
                else
                    err_quit("str_cli: server terminated prematurely");

                /* successfully read data from socket */
            } else {
                fprintf(stderr, "%s: read %ld bytes from socket\n", gf_time(), (long) n);
                /* advance the 'read from socket' pointer */
                friptr += n;
                /* Got new data; will need to write it. Next time,
                 * check STDOUT for availability. */
                FD_SET(STDOUT_FILENO, &wset);
            }
        }

        /* STDOUT output */
        if (FD_ISSET(STDOUT_FILENO, &wset) && ((n = friptr - froptr) > 0)) {
            if ((nwritten = write(STDOUT_FILENO, froptr, n)) < 0) {
                if (errno != EWOULDBLOCK)
                    err_sys("write() error on stdout");

            } else {
                fprintf(stderr, "%s: wrote %ld bytes to stdout\n", gf_time(), (long ) nwritten);
                /* advance 'write to stdout pointer */
                froptr += nwritten;
                /* if caught up with 'read from socket' pointer, 
                 * reset both to start of the 'from' buffer */
                if (froptr == friptr)
                    froptr = friptr = fr;
            }
        }

        /* socket write buffer */
        if (FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0)) {
            if ((nwritten = write(sockfd, tooptr, n)) < 0) {
                if (errno != EWOULDBLOCK)
                    err_sys("write() error on socket");

            } else {
                fprintf(stderr, "%s wrote %ld bytes to socket\n", gf_time(), (long) nwritten);
                /* advance the 'write to socket' pointer */
                tooptr += nwritten;
                /* if caught up with the 'read from STDIN' pointer,
                 * reset both to the start of the 'to' buffer */
                if (tooptr == toiptr) {
                    tooptr = toiptr = to;
                    /* if previously finished reading from STDIN,
                     * send FIN to server. No more data will come and
                     * we're done writing all we had. */
                    if (stdineof)
                        shutdown(sockfd, SHUT_WR);
                }
            }
        }
    }
}
