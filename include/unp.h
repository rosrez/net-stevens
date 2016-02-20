/* Our own header file */

#ifndef __ump_h
#define __ump_h

#include "config.h"      /* config params for this OS; config.h is generated by configure */

/* after changing #include directives, make sure that you update acsite.m4 as well */

#include <sys/types.h>      /* main system types */
#include <sys/socket.h>     /* main socket definitions */
#include <sys/time.h>       /* timeval{} struct for select() */
#include <time.h>           /* timespec{} for pselect() */
#include <netinet/in.h>     /* struct sockaddr_in{} and other network definitions */
#include <arpa/inet.h>      /* inet(3) functions */
#include <errno.h>
#include <fcntl.h>          /* for non-blocking sockets */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>       /* for S_xxx contstants */
#include <sys/uio.h>        /* for iovec{} struct and readv()/writev() */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>         /* for UNIX domain sockets */

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>     /* for convenience */
#endif

#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif

#ifdef HAVE_POLL_H
#include <poll.h>           /* for convenience */
#endif

#ifdef HAVE_SYS_EVENT_H
#include <sys/event.h>      /* for kqueue */
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>        /* for convenience */
#endif

/* 
 * Three header files are usually required to call ioctl()
 * for a socket/file: <sys/ioctl.h>, <sys/filio.h>, <sys/sockio.h>
 */

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#ifdef HAVE_SYS_SOCKIO_H
#include <sys/sockio.h>
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef HAVE_NET_IF_DL_H
#include <nent/if_dl.h>
#endif

#ifdef HAVE_NETINET_SCTP_H
#include <nnetinet/sctp.h>
#endif

/* OSF/1 essentially forbits recv() and send() in <sys/socket.h> */
#ifdef __osf__
#undef recv
#undef sennd
#define recv(a,b,c,d) recvfrom(a,b,c,d,0,0,)

#define send(a,b,c,d) sendto(a,b,c,d,0,0)
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff      /* has to be in <netinet/in.h> */
#endif

#ifndef SHUT_RD     /* three new consannts POSIX.1g */
#define SHUT_RD     0   /* shutdown read */
#define SHUT_WR     1   /* shutdown write */
#define SHUT_RDWR   2   /* shutdown read and write */
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN     16 /* "ddd.ddd.ddd.ddd\e0"
                                   1234567890123456 */
#endif

/* 
 * This is needed even if IPv6 is not supported so that we are always able
 * to place a buffer of required size without #ifdef directives.
 */
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN    46  /* maximum length of address string in IPv6:
xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx* OR
"xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\e0"
1234567890123456789012345678901234567890123456 */
#endif

/* define bzero() as a macro, if this function is not defined in the standard library */
#ifndef HAVE_BZERO
#define bzero(ptr, n)   memset(ptr, 0, n)
#endif

/* Older systems don't have gethostbyname2() */
#ifndef HAVE_GETHOSSTBYNAME2
#define gethostbyname2(host, family) gethostbyname(host)
#endif

#ifndef HAVE_IN_PKTINFO
/* Structure returned by recvfrom_flags() */
struct in_pktinfo {
    struct in_addr  ipi_addr;       /* IPv4 destination address */
    int             ipi_ifindex;    /* received interface index */ 
};
#endif

/* We need newer macros CMSG_LEN() and CMSG_SPACE(), but currently they are not
 * supported by many implementations. They need the ALIGN() macro, but this is
 * depends on implementation.
 */
#ifndef CMSG_LEN
#define CMSG_LEN(size)      (sizeof(struct cmsghdr) + (size))
#endif
#ifndef CMSG_SPACE
#define CMSG_SPACE(size)    (sizeof(struct cmsghdr) + (size))
#endif

/*
 * POSIX requires SUN_LEN() macros, but it not is defined by all implementations.
 * In 4.4BSD this macro works even if the length field is not present.
 */
#ifndef SUN_LEN
#define SUN_LEN(su) \
    (sizeof(*(su)) - sizeof((su)->sun_path) + strlen(su)->sun_path
#endif

/* 
 * In POSIX, "UNIX domain is called "local IPC", but
 * AF_LOCAL and PF_LOCAL are not currently defined by all systems.
 */

#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
#define PF_LOCAL PF_UNIX
#endif

/*
 * POSIX demands defining INFTIM in <poll.h>, but in many systems
 * it is still defined in <sys/stropts.h>. In order not to include
 * all the functions that work with streams, we define it here.
 * This is a standard value, but there is no guarantee that it is equal -1.
 */
#ifndef INFTIM
#define INFTIME (-1)    /* infinite timeout */
#ifdef HAVE_POLL_H
#define INFTIME_UNPH    /* we need to mention in unpxti.h that this constant
                           is defined here */

#endif
#endif

/*
 * We could have extracted this value from SOMAXCONN in <sys/socket.h>,
 * but many kernels still define it as 5, even though much larger values
 * are supported.
 */
#define LISTENQ 1024    /* second argument to listen() */

/* Miscelaneous constants */
#define MAXLINE     4096    /* maximum length of a text string */
#define BUFFSIZE    8192    /* buffer size for reads and writes */

/* Define port number that can be used for client-server interaction */
/* OXR: We use port 50000 for simplicity -- requires no root access */
#define SERV_PORT       50000    /* TCP and UDP clients and servers */
#define SERV_PORT_STR   "50000"  /* TCP and UDP clients and servers */
#if 0
#define SERV_PORT       9877    /* TCP and UDP clients and servers */
#define SERV_PORT_STR   "9877"  /* TCP and UDP clients and servers */
#endif
#define UNIXSTR_PATH    "/tmp/unix.str"     /* stream clients/servers - UNIX domain */
#define UNIXDG_PATH     "/tmp/unix.dg"      /* datagram clients/servers - UNIX domain */

/* The following defines simplify typecasts, i.e. require less typing */
#define SA struct sockaddr

#define HAVE_STRUCT_SOCKADDR_STORAGE
#ifndef HAVE_STRUCT_SOCKADDR_STORAGE
/*
 * RFC 3499: protocol-independent socket address structure.
 */
#define __SS_MAXSIZE    128
#define __SS_ALIGNSIZE  (sizeof(int64_t))
#ifndef HAVE_SOCKADDR_SA_LEN
#define __SS_PAD1SIZE    (__SS_ALIGNSIZE - sizeof(u_char) - sizeof(sa_family_t))
#else
#define __SS_PAD1SIZE   (__SS_ALIGNSIZE - sizeof(sa_family_t))
#endif

#define __SS_PAD2SIZE   (_SS_MAXSIZE - 2*__SS_ALIGNSIZE)

struct sockaddr_storage {
#ifdef HAVE_SOCKADDR_SA_LEN
    u_char  ss_len;
#endif
    sa_family_t ss_family;
    char        __ss_pad1[__SS_PAD1SIZE];
    int64_t     __ss_align;
    char        __ss_pad2[__SS_PAD2SIZE];
};
#endif

/* default permissions for new files */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default permissions for new directories */
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc (int); /* signal handlers */

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

#ifndef HAVE_ADDRINFO_STRUCT
#include "../lib/addrinfo.h"
#endif

#ifndef HAVE_IF_NAMEINDEX_STRUCT
struct if_nameindex {
    unsigned int if_index;  /* 1, 2, ... */
    char         *if_name;  /* null-terminated name */
};
#endif

#ifndef HAVE_TIMESPEC_STRUCT
struct timespec {
    time_t tv_sec;      /* seconds */
    long tv_nsec;       /* nanoseconds */
};
#endif

/* prototypes for our socket wrapper functions: see {Sec errors} */
int  Accept(int, SA *, socklen_t *);
void Bind(int, const SA *, socklen_t);
void Connect(int, const SA *, socklen_t);
void Close(int);
void Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr);
void Listen(int, int);
ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags);
ssize_t Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
         struct sockaddr *sa, socklen_t *salenptr);
ssize_t Recvmsg(int fd, struct msghdr *msg, int flags);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout);
void Send(int fd, const void *ptr, size_t nbytes, int flags);
void Sendto(int fd, const void *ptr, size_t nbytes, int flags,
       const struct sockaddr *sa, socklen_t salen);
void Sendmsg(int fd, const struct msghdr *msg, int flags);
void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
void Shutdown(int fd, int how);
int Sockatmark(int fd);
int Socket(int, int, int);
void Socketpair(int, int, int, int *);
void Writen(int, void *, size_t);

/* wrapunix.c */
void     Write(int, void *, size_t);

/* wrapunix.c */
int     Read(int, void *, size_t);

/* error.c */

void     err_dump(const char *, ...);
void     err_msg(const char *, ...);
void     err_quit(const char *, ...);
void     err_ret(const char *, ...);
void     err_sys(const char *, ...);

/* str_cli.c */
void str_cli_interactive(FILE *fp, int sockfd);
/* str_cli_select.c */
void str_cli_select(FILE *fp, int sockfd);

#ifdef STR_CLI_INTERACTIVE
#define str_cli str_cli_interactive
#else
#define str_cli str_cli_select
#endif

/* str_echo.c */
void str_echo(int sockfd);

/* readline.c */
ssize_t readline(int fd, void *vptr, size_t maxlen);
ssize_t readlinebuf(void **vptrptr);

/* readn.c */
ssize_t readn(int fd, void *vptr, size_t n);

/* writen.c */
ssize_t writen(int fd, const void *vptr, size_t n);

/* dg_cli.c */
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

/* dg_cli_connect.c */
void dg_cli_connect(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

/* dg_echo.c */
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

/* dg_echo_loop.c */
void dg_echo_loop(int sockfd, SA *pcliaddr, socklen_t clilen, int sleep_time);

/* dg_cli_loop.c */
void dg_cli_loop(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen, int dgcount);

/* host_serv.c */
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);

/* tcp_connect.c */
int tcp_connect(const char *hostname, const char *service);

/* tcp_listen.c */
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

/* sock_ntop.c */
char *sock_ntop(const struct sockaddr *sa, socklen_t salen);

/* sock_ntop_host.c */
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

/* sock_addr.c */
int sock_cmp_addr(const struct sockaddr *sockaddr1, const struct sockaddr *sockaddr2, socklen_t addrlen);
int sock_cmp_port(const struct sockaddr *sockaddr1, const struct sockaddr *sockaddr2, socklen_t addrlen);
int sock_get_port(const struct sockaddr *sockaddr, socklen_t addrlen);
void sock_set_addr(const struct sockaddr *sockaddr, socklen_t addrlen, void *ptr);
void sock_set_port(const struct sockaddr *sockaddr, socklen_t addrlen, int port);

/* sigchldwaitpid.c */
void sig_chld(int signo);

/* read_fd.c */
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

/* write_fd.c */
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

/* gf_time.c */
char *gf_time(void);

/* str_cli_nonb.c */
void str_cli_nonb(FILE *fp, int sockfd);

/* str_cli_fork.c */
void str_cli_fork(FILE *fp, int sockfd);

/* in_cksum.c */
uint16_t in_cksum(uint16_t *addr, int len);

#endif /* __ump_h */
