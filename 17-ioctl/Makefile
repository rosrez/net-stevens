QUIET?=@

TARGETNAMES = \
	prifinfo \
	prmac 

-include ../project.mak

$(BLDDIR)/prifinfo: prifinfo.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/prmac: prmac.o $(LIBSOCK)
	$(CC) -o $@ $^

