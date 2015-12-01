#include "unp.h"

#include <stdarg.h>
#include <syslog.h>     /* for syslog() */

int     daemon_proc;    /* is set to non-zero value with daemon_init() */

static void err_doit(int, int, const char *, va_list);

/*
 * Non-fatal error resulting from a system call.
 * Output error message and return.
 */
void err_ret(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}

/* 
 * Fatal error resulting from a system call.
 * Output error message and exit.
 */
void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
 * Fatal error resulting from a system call.
 * Output error message, save memory dump of 
 * the process and terminate.
 */
void err_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();    /* save the core dump and terminate. */
    exit(1);
}

/* 
 * Non-fatal error tha doesn't result from a system call.
 * Output error message and return.
 */
void err_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}

/* 
 * Fatal error that doesn't result from a system call.
 * Output error message and terminate.
 */
void err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

/* 
 * Output message and return.
 * The caller sets "errnoflag" and "level"
 */
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int     errno_save, n;
    char    buf[MAXLINE + 1];

    errno_save = errno;     /* the caller may need the value */

#ifdef HAVE_VSNPRINTF
    vsnprintf(buf, MAXLINE, fmt, ap);   /* protected version */
#else
    vsprintf(buf, fmt, ap);             /* unprotected version */
#endif
    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\en");

    if (daemon_proc) {
        syslog(level, buf);
    } else {
        fflush(stdout);     /* if stdout and stderr coinside */
        fputs(buf, stderr);
        fflush(stderr);
    }
}

