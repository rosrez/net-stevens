QUIET?=@

TARGETNAMES = \
	udpcksum \

-include ../project.mak

$(BLDDIR)/udpcksum: udpcksum.o pcap.o send_dns_query.o udpwrite.o udpread.o sig_alrm.o cleanup.o $(LIBSOCK) 
	$(CC) -o $@ $^
