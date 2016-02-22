// NeoException.h


#ifndef _NEOEXCEPTION_H_
#define _NEOEXCEPTION_H_

class CException {          // base class, don't use

protected:
    char* message;
    char* secondary;
        
public:
    CException(char* m) {message = m; secondary = 0;}
    CException(char* m, char* s) {message = m; secondary = s;}
    char* Diagnostic() {return message;}
    char* WhatFailed() {return secondary;}
    
};


class CFatalError : public CException {
public:
    CFatalError(char* m) : CException(m) {}
    CFatalError(char* m, char* s) : CException(m, s) {}
};


class CMemoryFullError : public CException {
public:
    CMemoryFullError(char* m) : CException(m) {}
    CMemoryFullError(char* m, char* s) : CException(m, s) {}
};

#endif // _NEOEXCEPTION_H_

