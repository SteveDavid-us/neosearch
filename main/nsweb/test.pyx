#!cython

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "CGiant.h":
    cdef cppclass CGiant:
        CGiant() except +

cdef extern from "CHitList.h":
    cdef cppclass hit:
        void Set(short v, short p, short w)

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

cdef class PyNS:
    def get_results(self, word):
        cdef CGiant *giantTable = new CGiant()
        cdef CTextFetch *textFetch = new CTextFetch()
        cdef CTextExploder *textExploder = new CTextExploder()
        cdef CHitOffsetList *hitOffsetList = new CHitOffsetList()
        textExploder.Setup(textFetch, giantTable, hitOffsetList)
