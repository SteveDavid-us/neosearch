// *******************************************************************************************
// CResultBuilder.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 2016 -- JPS
// -------------------------------------------------------------------------------------------
// Class that processes generates json output
//
// *******************************************************************************************

#ifndef _CRESULTBUILDER_H_
#define _CRESULTBUILDER_H_

#include <jsoncpp/json/json.h>

#include "CGiant.h"
#include "CTextFetch.h"
#include "CHitList.h"

using namespace std;

// ###########################################################################################
class CResultBuilder {
public:
    CResultBuilder() {
    }

    std::string Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList);

};
// ###########################################################################################

#endif // _CRESULTBUILDER_H_
