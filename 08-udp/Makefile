QUIET?=@

-include ../project.mak

TARGETNAMES = \
	udpserv01 \
	udpcli01 \
	udpcli_connect \
	udpserv_loop \
	udpcli_loop

TARGETS := $(addprefix $(BLDDIR)/, $(TARGETNAMES))

all: $(TARGETS)

$(BLDDIR)/udpserv01: udpserv01.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/udpcli01: udpcli01.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/udpcli_connect: udpcli_connect.o $(LIBSOCK)
	$(CC) -o $@ $^

$(BLDDIR)/udpserv_loop: udpserv_loop.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/udpcli_loop: udpcli_loop.o $(LIBSOCK)
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
