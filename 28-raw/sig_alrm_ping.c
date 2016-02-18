#include "ping.h"

void sig_alrm(int signo)
{
    /* call the protocol-dependent function to send an ICMP echo request */
    (*pr->fsend)();

    /* schedule another SIGALRM for 1 second in the future */
    alarm(1);
    return;
}
