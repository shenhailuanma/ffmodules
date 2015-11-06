
ifeq ($(MAKELEVEL),0)
.PHONY: all
all:
	$(MAKE) tests -C ..
clean:
	rm -rf *.o *.d 
else

CPPFLAGS = -I. -I$(INSTALLDIR)/include \
           -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D BUILD_TIME=\"$(shell date +%Y-%m-%d)\"
####CFLAGS += -Wall -Werror -std=c99
CFLAGS += -Wall -std=c99
LDFLAGS += -lpthread -ldl -lrt

Source := $(wildcard *.c)
#Source := cJSON.c tvie_matrix.c tvie_module_funs.c 
OBJECTS := $(patsubst %.c, %.o, $(Source))



.PHONY: all

all: test_fifo
	mkdir -p $(INSTALLDIR)/bin
	install -m 755 test_fifo $(INSTALLDIR)/bin/test_fifo

test_fifo: $(OBJECTS) $(INSTALLDIR)/lib/libffmodules.a 
	$(CC) $^ -o $@ -lasound -lm -lz -lbz2 -lssl -lcrypto $(CPPFLAGS) $(LDFLAGS) $(LINKFLAGS)

clean:
	rm -rf *.o

endif
