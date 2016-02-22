QUIET?=@

TARGETNAMES = \
	tcpcli_thread

-include ../project.mak

$(BLDDIR)/tcpcli_thread: tcpcli_thread.o str_cli_thread.o $(LIBSOCK) $(LIBTHREAD)
	$(CC) -o $@ $^ -lpthread
