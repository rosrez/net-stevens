QUIET?=@

# project.mak relies on TARGETNAMES, so this must be defined before we include project.mak
TARGETNAMES = \
	unixbind \
	unixstrcli \
	unixstrserv \
	unixdgcli \
	unixdgserv \
	mycat \
	openfile

-include ../project.mak

$(BLDDIR)/unixbind: unixbind.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/unixstrcli: unixstrcli.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/unixstrserv: unixstrserv.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/unixdgcli: unixdgcli.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/unixdgserv: unixdgserv.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/mycat: mycat.o my_open.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/openfile: openfile.o $(LIBSOCK)
	$(CC) -o $@ $^
