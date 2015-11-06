# make or make D=release        build _release version
# make D=debug                  build _debug version
# make D=debug_prof             build _debug_prof version
# make D=static_release         build _static_release version
# make D=static_debug           build _static_debug version

ifndef D
 # default: _release version
 D = release
endif

# if you want any binary to be linked statically in static_release and static_debug, append link command with $(LINKFLAGS)
ifeq ($(D),release)
 # _release version
 #CFLAGS = -g -O3
 CFLAGS = -O3
 LDFLAGS =
 LINKFLAGS =
else ifeq ($(D),debug)
 # _debug version
 CFLAGS = -g -O0 -DTVIE_DEBUG
 LDFLAGS =
 LINKFLAGS =
else ifeq ($(D),debug_prof)
 # _dyndebug.prof version
 CFLAGS = -g -O0 -DTVIE_DEBUG -pg
 # TODO: the following profile options are not supported by x264, I don't know why:
 # -fprofile-arcs -ftest-coverage
 LDFLAGS = -pg
 LINKFLAGS =
else ifeq ($(D),static_release)
 # _dyndebug version
 CFLAGS = -g -O3
 LDFLAGS =
 LINKFLAGS = -static -ldl
else ifeq ($(D),static_debug)
 # _dyndebug version
 CFLAGS = -g -O0 -DTVIE_DEBUG
 LDFLAGS =
 LINKFLAGS = -static -ldl
else
 $(error invalid D value $(D))
endif

export INSTALLDIR := $(CURDIR)/_$(D)
export CFLAGS
export CXXFLAGS = $(CFLAGS)
export LDFLAGS
export LINKFLAGS

#targets = 3rdparty modules tests
targets = modules  tests


.PHONY: $(targets) all clean 

all: $(targets) 

$(targets):
	$(MAKE) -C $@

clean:
	svn status --no-ignore | grep '^\?\|^I' | sed 's/^\(\?\|I\) \+//' | sed 's/.*/"&"/' | xargs -n 20 rm -rf

install: all
	sudo mkdir -p /usr/local/ffmodules/
