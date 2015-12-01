INCLUDE_DIRS = ../lib
INCLUDE := $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS += -g -Wall
CPPFLAGS += $(INCLUDE)

VPATH = ../lib
