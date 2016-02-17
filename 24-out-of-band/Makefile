QUIET?=@

TARGETNAMES = \
	send-oob \
	send-oob-1byte \
	send-oob-big \
	recv-oob \
	recv-oob-select \
	recv-oob-mark \
	recv-oob-big \

-include ../project.mak

$(BLDDIR)/send-oob: send-oob.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/send-oob-1byte: send-oob-1byte.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/send-oob-big: send-oob-big.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/recv-oob: recv-oob.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/recv-oob-select: recv-oob-select.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/recv-oob-mark: recv-oob-mark.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/recv-oob-big: recv-oob-big.o $(LIBSOCK) 
	$(CC) -o $@ $^
