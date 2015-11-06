
ifeq ($(MAKELEVEL),0)
.PHONY: all
all:
	$(MAKE) ffmodules -C ..
clean:
	rm -rf *.o *.d *.a
else

CPPFLAGS = -I. -I$(INSTALLDIR)/include \
           -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE
####CFLAGS += -Wall -Werror -std=c99
CFLAGS += -Wall -std=c99
LDFLAGS += -lpthread -ldl

Source := $(wildcard *.c)
#Source := sqlite3.c tvie_buffer.c tvie_fifo.c tvie_module.c tvie_module_common.c tvie_module_delay.c 
OBJECTS := $(patsubst %.c, %.o, $(Source))
HEADERS := $(patsubst %.c, %.h, $(Source))


BUILD_ARCH = $(shell uname -m)

.PHONY: all

Dest := libffmodules.a

all: $(Dest)
	mkdir -p $(INSTALLDIR)/lib
	install -m 664 $(Dest) $(INSTALLDIR)/lib/$(Dest)
	install -m 664 $(HEADERS) "$(INSTALLDIR)/include/"
$(Dest): $(OBJECTS)
	ar rcs $@ $?
clean:
	rm -rf *.o *.d $(Dest)
#include ../autodepend.mak

endif
