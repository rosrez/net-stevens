QUIET?=@

# project.mak relies on TARGETNAMES, so this must be defined before we include project.mak
TARGETNAMES = \
	daytimetcpcli_names \
#	daytimetcpsrv

-include ../project.mak

#$(BLDDIR)/daytimetcpsrv: daytimetcpsrv.o $(LIBSOCK) 
#	$(CC) -o $@ $^

$(BLDDIR)/daytimetcpcli_names: daytimetcpcli_names.o $(LIBSOCK)
	$(CC) -o $@ $^
