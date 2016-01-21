QUIET=@

include ../project.mak

LIBS := $(LIBNAME)

LIBOBJ = $(subst .c,.o,$(wildcard *.c))

all: $(LIBS)

$(LIBNAME): $(OBJ)
	$(AR) r $@ $?

.PHONY: clean

clean:
	$(QUIET)rm -f $(LIBS)
	$(QUIET)rm -f *.o *.d *.tmp
