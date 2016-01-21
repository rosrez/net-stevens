# Build directory (where executables reside)
BLDDIR = ../bin

# Include directories
INCLUDE_DIRS := ../include
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

# Compile/link flags
CFLAGS += -g -Wall 
CPPFLAGS += $(INCLUDES)

# Library framework
LIBDIR=../lib
LIBSOCK = $(LIBDIR)/libsock.a
