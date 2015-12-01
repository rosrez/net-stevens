QUIET?=@

include ../Makefile

TARGETS = \
	daytimetcpcli \
	daytimetcpsrv

all: $(TARGETS)

daytimetcpsrv: wrapsock.o wrapunix.o error.o

daytimetcpcli: error.o

.PHONY: clean

clean:
	$(QUIET)rm -f $(TARGETS)
	$(QUIET)rm -f *.o *.d *.tmp
