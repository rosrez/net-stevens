QUIET?=@

TARGETNAMES = \
	tcpcli_nonb \
	tcpcli_fork

-include ../project.mak

$(BLDDIR)/tcpcli_nonb: tcpcli_nonb.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/tcpcli_fork: tcpcli_fork.o $(LIBSOCK)
	$(CC) -o $@ $^
