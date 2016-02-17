QUIET?=@

TARGETNAMES = \
	send-oob \
	recv-oob \

-include ../project.mak

$(BLDDIR)/send-oob: send-oob.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/recv-oob: recv-oob.o $(LIBSOCK) 
	$(CC) -o $@ $^
