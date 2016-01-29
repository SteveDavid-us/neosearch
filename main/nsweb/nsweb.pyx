#!cython

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "Types.h":
    pass

cdef extern from "CTarget.h":
    cdef cppclass CTarget:
        CTarget() except +
        void SetGrammarDoCare() except +
        void ClearGrammar() except +
        void SetString(const char *theString) except +
        void SetStemDontCare() except +
        void SetSearchType(short s) except +
        void SetAmbChecking(bool a) except +
        void SetDisambChecking(bool d) except +

cdef extern from "CGiant.h":
    cdef cppclass CGiant:
        CGiant() except +
        void SetDisambiguationChecking(bool setting) except +
        void SetAmbiguityChecking(bool setting) except +
        void SetSearchMode(int mode, CHitList *oldList, int proxim) except +
        CHitList* FindHits(CTarget theTarget) except +
        CHitList* FindHitsWithMatchOf(CTarget theTarget) except +

cdef extern from "CHitList.h":
    cdef cppclass hit:
        void Set(short v, short p, short w)

    cdef cppclass CHitList:
        CHitList() except +
        long ReportTotalHits() except +

cdef extern from "CTextFetch.h":
    cdef cppclass CTextFetch:
        CTextFetch() except +

cdef extern from "CHitOffsetList.h":
    cdef cppclass CHitOffsetList:
        CHitOffsetList() except +

cdef extern from "CTextExploder.h":
    cdef cppclass CTextExploder:
        CTextExploder() except +
        void Setup(CTextFetch *t, CGiant *g, CHitOffsetList *l) except +

cdef extern from "Engine.h":
    cdef int CREATE_NEW
    cdef int UNION
    cdef int INTERSECT

cdef extern from "CResultBuilder.h":
    cdef cppclass CResultBuilder:
        CResultBuilder() except +
        string Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList) except +

cdef class NeoEngine:
    cdef CGiant *giantTable
    cdef CTextFetch *textFetch
    cdef CTextExploder *textExploder
    cdef CHitList *hitList
    cdef CTarget *target

    def __cinit__(self):
        self.giantTable = new CGiant()
        self.textFetch = new CTextFetch()
        self.textExploder = new CTextExploder()
        self.target = new CTarget()
        self.hitList = NULL
    
    def __dealloc__(self):
        del self.textExploder
        del self.textFetch
        del self.giantTable
        del self.hitList
        del self.target
    
    def match(self, term):
        self.target.SetString(term)
        self.giantTable.SetAmbiguityChecking(1)
        self.giantTable.SetDisambiguationChecking(0)
        self.giantTable.SetSearchMode(CREATE_NEW, self.hitList, 25)
        self.hitList = self.giantTable.FindHitsWithMatchOf(deref(self.target))
        resultBuilder = new CResultBuilder()
        return resultBuilder.Write(self.giantTable, self.textFetch, self.hitList).decode('UTF-8')

