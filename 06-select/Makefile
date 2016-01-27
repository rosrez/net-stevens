QUIET?=@

TARGETNAMES = \
	tcpservselect01 

-include ../project.mak

$(BLDDIR)/tcpservselect01: tcpservselect01.o $(LIBSOCK) 
	$(CC) -o $@ $^
