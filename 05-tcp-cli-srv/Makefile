QUIET?=@

-include ../project.mak

TARGETNAMES = \
	tcpserv01 \
	tcpcli01

TARGETS := $(addprefix $(BLDDIR)/, $(TARGETNAMES))

all: $(TARGETS)

$(BLDDIR)/tcpserv01: tcpserv01.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/tcpcli01: tcpcli01.o $(LIBSOCK)
	$(CC) -o $@ $^

# This include contains targets, so include after the default target
-include ../lib.mak

.PHONY: clean

clean:
	$(QUIET)rm -f $(TARGETS)
	$(QUIET)rm -f *.o *.d *.tmp

distclean: clean
	$(QUIET)rm -f $(LIBS)
	$(QUIET)rm -f $(LIBOBJ)
