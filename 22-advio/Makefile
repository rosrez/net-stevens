QUIET?=@

TARGETNAMES = \
	udpserv_if \

-include ../project.mak

$(BLDDIR)/udpserv_if: udpserv_if.o $(LIBSOCK) 
	$(CC) -o $@ $^
