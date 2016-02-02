QUIET?=@

TARGETNAMES = \
	udpcli_bcast \

-include ../project.mak

$(BLDDIR)/udpcli_bcast: udpcli_bcast.o dgcli_bcast.o $(LIBSOCK)
	$(CC) -o $@ $^


