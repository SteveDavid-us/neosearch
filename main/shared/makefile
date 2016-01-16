# work in progress makefile for daisy	

#	name for executable
_APP = ns2

# add all obj's here!
_OBJS = CDataFile.o QSortRand.o CHitList.o CDictionary.o CAmbiguity.o CDisAmbTable.o CGenericProgress.o CTarget.o CGiant.o CHitOffsetList.o CTextFetch.o CTextExploder.o StringTools.o

OBJS = $(patsubst %,$(OUTDIR)/%,$(_OBJS))

CXX = g++
OUTDIR = bin
APP = $(OUTDIR)/$(_APP)
#OBJS = $(patsubst %,$(OUTDIR)/%,$(_OBJS))

#	wx-config
#WXCONFIG = /usr/local/$(OUTDIR)/wx-config


#	flags and libraries
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2 -Wall
PROFILE_FLAGS=-pg -O2

FLAGS=-Iinclude 
LIBS= 

#	platform-specific makefile includes
#SYSTEM=$(shell config/config.guess | cut -d - -f 3 | sed -e 's/[0-9\.]//g;')
#SYSTEM.SUPPORTED = $(shell test -f config/Makefile.$(SYSTEM) && echo 1)
#ifeq ($(SYSTEM.SUPPORTED), 1)
#include config/Makefile.$(SYSTEM)
#else
#$(error "Platform '$(SYSTEM)' not supported")
#endif

#	make targets
all : debug
#release : FLAGS += $(RELEASE_FLAGS)
#release : $(APP)
#release : setup
debug : FLAGS += $(DEBUG_FLAGS)
debug : $(APP)
debug : ./$(OUTDIR) 
#profile : FLAGS += $(PROFILE_FLAGS)
#profile : $(APP)
#profile : setup

./$(OUTDIR) :
	mkdir -p $(OUTDIR)

$(OUTDIR)/CGiant.o: src/CGiant.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o $(OUTDIR)/CAmbiguity.o $(OUTDIR)/CHitList.o $(OUTDIR)/CTarget.o $(OUTDIR)/CGenericProgress.o $(OUTDIR)/StringTools.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CHitList.o: src/CHitList.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CTextExploder.o: src/CTextExploder.cpp $(OUTDIR)/CTextFetch.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CTextFetch.o: src/CTextFetch.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CDisAmbTable.o: src/CDisAmbTable.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CDictionary.o: src/CDictionary.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CAmbiguity.o: src/CAmbiguity.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/%.o : src/%.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

$(APP): $(OBJS) src/tests/TextBrowse.cpp
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/*.o $(APP) $(APP).exe

