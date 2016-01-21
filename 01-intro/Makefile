QUIET?=@

-include ../project.mak

TARGETNAMES = \
	daytimetcpcli \
	daytimetcpsrv

TARGETS := $(addprefix $(BLDDIR)/, $(TARGETNAMES))

all: $(TARGETS)

$(BLDDIR)/daytimetcpsrv: daytimetcpsrv.o $(LIBSOCK) 
	$(CC) -o $@ $^

$(BLDDIR)/daytimetcpcli: daytimetcpsrv.o $(LIBSOCK)
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
