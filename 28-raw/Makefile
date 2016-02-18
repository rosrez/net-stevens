QUIET?=@

TARGETNAMES = \
	ping \
	traceroute \

-include ../project.mak

$(BLDDIR)/ping: ping.o readloop.o proc_v4.o proc_v6.o send_v4.o send_v6.o init_v6.o sig_alrm_ping.o tv_sub.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/traceroute: traceroute.o traceloop.o recv_v4.o recv_v6.o \
		icmpcode_v4.o icmpcode_v6.o sig_alrm_trace.o tv_sub.o $(LIBSOCK) 
	$(CC) -o $@ $^
