#	single-executable makefile

#	name for executable
_APP = t1
_OBJS = t1.o

#	put all obj's here, sources expected in project root as *.cpp
#_OBJS = $(patsubst %,src/gui/%,$(_GUI)) $(patsubst %,src/core/%,$(_CORE))

CXX = g++
OUTDIR = bin
APP = $(OUTDIR)/$(_APP)
OBJS = $(patsubst %,$(OUTDIR)/%,$(_OBJS))

#	wx-config
WXCONFIG = /usr/local/bin/wx-config


#	flags and libraries
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2 -Wall
PROFILE_FLAGS = -pg -O2

WX_INCL = $(shell $(WXCONFIG) --cxxflags)
WX_LIBS = $(shell $(WXCONFIG) --libs) 

FLAGS = -Iinclude $(WX_INCL)
LIBS = $(WX_LIBS)

#	platform-specific makefile includes
SYSTEM = $(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')
SYSTEM.SUPPORTED = $(shell test -f config/Makefile.$(SYSTEM) && echo 1)
ifeq ($(SYSTEM.SUPPORTED), 1)
include config/Makefile.$(SYSTEM)
else
$(error "Platform '$(SYSTEM)' not supported")
endif

#	make targets
all : debug
release : FLAGS += $(RELEASE_FLAGS)
release : $(APP)
debug : FLAGS += $(DEBUG_FLAGS)
debug : $(APP)
profile : FLAGS += $(PROFILE_FLAGS)
profile : $(APP)

$(OUTDIR)/%.o : src/%.cxx
	$(CXX) $(FLAGS) -c -o $@ $<

$(APP): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/*.o $(APP) $(APP).exe

