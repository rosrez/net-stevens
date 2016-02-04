QUIET?=@

TARGETNAMES = \
	udpserv01 \
	udpcli01 \
	udpcli_connect \
	udpserv_loop \
	udpcli_loop \
	udpcli_srcaddr

-include ../project.mak

$(BLDDIR)/udpserv01: udpserv01.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/udpcli01: udpcli01.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/udpcli_connect: udpcli_connect.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/udpserv_loop: udpserv_loop.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/udpcli_loop: udpcli_loop.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/udpcli_srcaddr: udpcli_srcaddr.o $(LIBSOCK)
	$(CC) -o $@ $^
