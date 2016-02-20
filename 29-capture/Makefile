QUIET?=@

LFLAGS=-lpcap

TARGETNAMES = \
	udpcksum \

-include ../project.mak

$(BLDDIR)/udpcksum: udpcksum.o pcap.o test_udp.o send_dns_query.o udpwrite.o udpread.o cleanup.o $(LIBSOCK) 
	$(CC) -o $@ $^ $(LFLAGS)
