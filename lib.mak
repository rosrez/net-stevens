LIBS = $(LIBSOCK)

LIBOBJ = $(subst .c,.o,$(wildcard $(LIBDIR)/*.c))

$(LIBSOCK): $(LIBOBJ)
	$(AR) r $@ $?
