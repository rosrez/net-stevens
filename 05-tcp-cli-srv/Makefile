QUIET?=@

TARGETNAMES = \
	tcpserv01 \
	tcpcli01

-include ../project.mak

$(BLDDIR)/tcpserv01: tcpserv01.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/tcpcli01: tcpcli01.o $(LIBSOCK)
	$(CC) -o $@ $^
