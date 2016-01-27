QUIET?=@

# project.mak relies on TARGETNAMES, so this must be defined before we include project.mak
TARGETNAMES = \
	daytimetcpcli \
	daytimetcpsrv

-include ../project.mak

$(BLDDIR)/daytimetcpsrv: daytimetcpsrv.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/daytimetcpcli: daytimetcpcli.o $(LIBSOCK)
	$(CC) -o $@ $^
