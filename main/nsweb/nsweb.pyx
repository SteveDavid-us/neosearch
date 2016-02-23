#!cython

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.set cimport set as cppset

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

        void SetCase(short a) except +
        void SetParticiple(short a) except +
        void SetNumber(short a) except +
        void SetGender(short a) except +

        void SetMood(short a) except +
        void SetVoice(short a) except +
        void SetTense(short a) except +
        void SetPerson(short a) except +

cdef extern from "CGiant.h":
    cdef cppclass CGiant:
        CGiant() except +
        void SetDisambiguationChecking(bool setting) except +
        void SetAmbiguityChecking(bool setting) except +
        void SetSearchMode(int mode, CHitList *oldList, int proxim) except +
        CHitList* FindHits(CTarget theTarget) except +

cdef extern from "CHitList.h":
    cdef cppclass hit:
        void Set(short v, short p, short w)

    cdef cppclass CHitList:
        CHitList() except +
        long ReportTotalHits() except +

cdef extern from "CTextFetch.h":
    cdef cppclass CTextFetch:
        CTextFetch() except +
        char* GetBookName(short vol) except +

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
    cdef int MAX_VOLUMES

cdef extern from "CRoseCode.h":
    cdef int NO_STEM
    cdef int SUBSTANTIVE_STEM
    cdef int PRESENT_STEM
    cdef int PERFECT_ACTIVE_STEM
    cdef int PERFECT_PASSIVE_PPLE_STEM
    cdef int GERUNDIVE_STEM
    cdef int PRESENT_ACTIVE_PPLE_STEM
    cdef int FUTURE_ACTIVE_PPLE_STEM
    cdef int COMPAR_ADV_ADJ_STEM
    cdef int SUPERL_ADV_ADJ_STEM
    cdef int COMPAR_PERFECT_PASSIVE_PPLE_STEM
    cdef int SUPERL_PERFECT_PASSIVE_PPLE_STEM
    cdef int COMPAR_PRESENT_ACTIVE_PPLE_STEM
    cdef int SUPERL_PRESENT_ACTIVE_PPLE_STEM

    cdef int NO_CASE
    cdef int NOMINATIVE_CASE
    cdef int GENITIVE_CASE
    cdef int DATIVE_CASE
    cdef int ACCUSATIVE_CASE
    cdef int ABLATIVE_CASE
    cdef int VOCATIVE_CASE
    cdef int LOCATIVE_CASE

    cdef int NO_MOOD
    cdef int INDICATIVE_MOOD
    cdef int SUBJUNCTIVE_MOOD
    cdef int INFINITIVE_MOOD
    cdef int IMPERATIVE_MOOD

    cdef int NO_TENSE
    cdef int PRESENT_TENSE
    cdef int IMPERFECT_TENSE
    cdef int FUTURE_TENSE
    cdef int PERFECT_TENSE

    cdef int PLUPERFECT_TENSE
    cdef int FUTURE_PERFECT_TENSE
 
    cdef int NO_VOICE
    cdef int ACTIVE_VOICE
    cdef int PASSIVE_VOICE
  
    cdef int NO_PERSON
    cdef int FIRST_PERSON
    cdef int SECOND_PERSON
    cdef int THIRD_PERSON
   
    cdef int NO_NUMBER
    cdef int SINGULAR_NUMBER
    cdef int PLURAL_NUMBER
    
    cdef int NO_GENDER
    cdef int MASCULINE_GENDER
    cdef int FEMININE_GENDER
    cdef int NEUTER_GENDER                     

cdef extern from "CResultBuilder.h":
    cdef cppclass CResultBuilder:
        CResultBuilder() except +
        unsigned int firstPassage
        unsigned int passageCount
        cppset[unsigned int] volumeFilter
        
        string Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList) except +

def volumes():
    vols = list()
    cdef CTextFetch *textExploder
    textFetch = new CTextFetch()
    for i in range(MAX_VOLUMES):
        vols.append(textFetch.GetBookName(i))
    del textFetch
    return vols

cdef class NeoEngine:
    cdef CGiant *giantTable
    cdef CTextFetch *textFetch
    cdef CTextExploder *textExploder
    cdef CHitList *hitList

    def __cinit__(self):
        self.giantTable = new CGiant()
        self.textFetch = new CTextFetch()
        self.textExploder = new CTextExploder()
        self.hitList = NULL
    
    def __dealloc__(self):
        del self.textExploder
        del self.textFetch
        del self.giantTable
        del self.hitList
    
    def search(self, query):
        cdef int searchMode = CREATE_NEW
        cdef int proximity = 25
        cdef CTarget *target
        intersect_mode = 'proximity' in query and query['proximity']['enable']
        if intersect_mode:
            proximity = query['proximity']['words']
            
        self.giantTable.SetAmbiguityChecking(1)
        self.giantTable.SetDisambiguationChecking(0)
        for param in query['params']: 
            target = new CTarget()
            text = param['text'].lower().strip()
            if param['type'] == 'text':
                if not text:
                    del target
                    continue
                target.SetString(text)
            elif param['type'] == 'substantive':
                substantive = param['substantive']
                target.SetGrammarDoCare()
                if text:
                    target.SetString(text)
                if 'case' in substantive:
                    target.SetCase(substantive['case'])
                if 'participle' in substantive:
                    target.SetParticiple(substantive['participle'])
                if 'number' in substantive:
                    target.SetNumber(substantive['number'])
                if 'gender' in substantive:
                    target.SetGender(substantive['gender'])
            else:
                assert param['type'] == 'verb'
                verb = param['verb']
                target.SetGrammarDoCare()
                if text:
                    target.SetString(text)
                if 'mood' in verb:
                    target.SetMood(verb['mood'])
                if 'voice' in verb:
                    target.SetVoice(verb['voice'])
                if 'number' in verb:
                    target.SetNumber(verb['number'])
                if 'tense' in verb:
                    target.SetTense(verb['tense'])
                if 'person' in verb:
                    target.SetPerson(verb['person'])

            self.giantTable.SetSearchMode(searchMode, self.hitList, proximity)
            self.hitList = self.giantTable.FindHits(deref(target))

            del target
            if intersect_mode:
                searchMode = INTERSECT
            else:
                searchMode = UNION
            
        resultBuilder = new CResultBuilder()
        resultBuilder.firstPassage = query['first']
        resultBuilder.passageCount = query['count']
        if 'volumes' in query:
            for vol in query['volumes']:
                resultBuilder.volumeFilter.insert(vol)

        return resultBuilder.Write(self.giantTable, self.textFetch, self.hitList).decode('UTF-8')

