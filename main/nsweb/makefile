# work in progress makefile for daisy	

#	name for executable
_APP = webtest

# add all obj's here!
_OBJS = CDataFile.o QSortRand.o CHitList.o CDictionary.o CAmbiguity.o CDisAmbTable.o CGenericProgress.o CTarget.o CGiant.o CHitOffsetList.o CTextFetch.o CTextExploder.o StringTools.o CResultBuilder.o

OBJS = $(patsubst %,$(OUTDIR)/%,$(_OBJS))

CXX = g++
OUTDIR = bin
APP = $(OUTDIR)/$(_APP)
#OBJS = $(patsubst %,$(OUTDIR)/%,$(_OBJS))

#	wx-config
#WXCONFIG = /usr/local/$(OUTDIR)/wx-config


#	flags and libraries
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2
PROFILE_FLAGS=-pg -O2

FLAGS=-I../shared/include -Iinclude -Wno-write-strings
LIBS=-ljsoncpp

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
debug : ./$(OUTDIR) 
debug : $(APP)
#profile : FLAGS += $(PROFILE_FLAGS)
#profile : $(APP)
#profile : setup

./$(OUTDIR) :
	mkdir -p $(OUTDIR)

$(OUTDIR)/CGiant.o: ../shared/src/CGiant.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o $(OUTDIR)/CAmbiguity.o $(OUTDIR)/CHitList.o $(OUTDIR)/CTarget.o $(OUTDIR)/CGenericProgress.o $(OUTDIR)/StringTools.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CHitList.o: ../shared/src/CHitList.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CTextExploder.o: ../shared/src/CTextExploder.cpp $(OUTDIR)/CTextFetch.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CTextFetch.o: ../shared/src/CTextFetch.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CDisAmbTable.o: ../shared/src/CDisAmbTable.cpp $(OUTDIR)/CDataFile.o $(OUTDIR)/QSortRand.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CDictionary.o: ../shared/src/CDictionary.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CAmbiguity.o: ../shared/src/CAmbiguity.cpp $(OUTDIR)/CDataFile.o
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/CResultBuilder.o: CResultBuilder.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

$(OUTDIR)/%.o : ../shared/src/%.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

$(APP): $(OBJS) WebTest.cpp
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/*.o $(APP) $(APP).exe

