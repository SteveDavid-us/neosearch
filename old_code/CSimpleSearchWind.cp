// *******************************************************************************************
// CSimpleSearchWind.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************


#include "CSimpleSearchWind.h"
#include "NeoResourceConstants.h"
#include "NeoException.h"
#include "IndexerCodes.h"
#include "NeoUtils.h"
#include "CHitList.h"
#include "CTarget.h"

#include <LStream.h>
#include <LTextEdit.h>
#include <LStdControl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



//==================================================================================

CSimpleSearchWind* CSimpleSearchWind::CreateSearchWindStream(LStream *inStream)
{
    return (NEW CSimpleSearchWind(inStream));
}

//==================================================================================
//  Default constructor
/*
CSimpleSearchWind::CSimpleSearchWind()
{
}
*/

//==================================================================================
//  Construct-from-stream constructor.

CSimpleSearchWind::CSimpleSearchWind(LStream *inStream)
    : LWindow(inStream)
{
    mainApp = NULL;
}

//==================================================================================
//  Destructor

CSimpleSearchWind::~CSimpleSearchWind()
{
}

//==================================================================================

void CSimpleSearchWind::FinishCreateSelf()
{
    familyCheckBox = (LStdCheckBox*) FindPaneByID(Family_Check);
    ThrowIfNULL_(familyCheckBox); 
    familyCheckBox->AddListener(this);
    
    substringCheckBox = (LStdCheckBox*) FindPaneByID(Substring_Check);
    ThrowIfNULL_(substringCheckBox);

    wordField = (CNeoEditField*) FindPaneByID(Word_Edit_Simple);
    ThrowIfNULL_(wordField);
    wordField->AddListener(this);
    SwitchTarget(wordField);

    proximityField = (LEditField*) FindPaneByID(Proximity_Box_Simple);
    ThrowIfNULL_(proximityField);
        
    LStdButton *theButton = (LStdButton*) FindPaneByID(Start_Simple_Search);
    ThrowIfNULL_( theButton );
    theButton->AddListener(this);
}

//==================================================================================
//      ¥ ListenToMessage

void CSimpleSearchWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
    long    value = *(long *)ioParam;
    int     i;
    
    switch (inMessage) {
        case Return_Key_Pressed:
        case Start_Simple_Search:
            try {
                DoSimpleSearch();
            } catch (CMemoryFullError) {
                SignalPStr_("\pMemory is full!  NeoSearch will attempt to continue but may"
                            " not function correctly.");
            }
            break;

        case Family_Check:
            if (value)
                substringCheckBox->Disable();
            else
                substringCheckBox->Enable();
            break;

    }
}


//==================================================================================

void CSimpleSearchWind::DoSimpleSearch()
{
    CHitList        *theHits;
    char            *searchString;
    Str255          pSearchString;
    Boolean         checkFamily;
    Boolean         substringSearch;
    int             mode, prox;
    CTarget         theTarget;

    SetWatchCursor(TRUE);
    wordField->GetDescriptor(pSearchString);
    searchString = PascalToC(pSearchString);

    int loop;
    for (loop = 0; (searchString[loop]==' ') || (searchString[loop]=='\t'); loop++);
    if (searchString[loop]=='\0') {
        delete[] searchString;
        return;
    }

    checkFamily = familyCheckBox->GetValue();
    substringSearch = substringCheckBox->GetValue();
    prox = proximityField->GetValue();
    
    mainApp->giantTable->SetAmbiguityChecking(TRUE);
    mainApp->giantTable->SetDisambiguationChecking(FALSE);
    mode = CREATE_NEW;
    theHits = NULL;

    Boolean done = FALSE;
    Boolean badSearch = FALSE;
    int     stringPos = 0;
    char    token[30];
    
    do {
        done = GetNextLowerCaseWord(token, searchString, stringPos);
        if (!strcmp(token, "and") || !strcmp(token, "&")) {
            mode = INTERSECT;
            continue;
        } else if (!strcmp(token, "or") || !strcmp(token, "|")) {
            mode = UNION; 
            continue;
        }

        mainApp->giantTable->SetSearchMode(mode, theHits, prox);
        theTarget.SetString(token);
        
        if (checkFamily) {
            theHits = mainApp->giantTable->FindHitsWithLemmaOf(theTarget);
            int     error;
            Str255  pString;
            error = mainApp->giantTable->CheckForError();
            if (error == EXT_AMB_SEARCH_ERROR) {
                CToPascal(pString, token);
                ParamText(pString,"\pit is externally ambiguous", NULL, NULL);
                StopAlert(Search_Nope_Dialog, NULL);
                badSearch = TRUE;
            } else if (error == NON_PLACE_SEARCH_ERROR) {
                CToPascal(pString, token);
                ParamText(pString,"\pit has no grammatical meaning", NULL, NULL);
                StopAlert(Search_Nope_Dialog, NULL);
                badSearch = TRUE;
            }
        } else if (substringSearch)
            theHits = mainApp->giantTable->FindHitsWithSubstringOf(theTarget);
        else
            theHits = mainApp->giantTable->FindHitsWithStringOf(theTarget);

        mode = INTERSECT;   // in case of implicit AND (no conjunction given)
    
    } while (!done);
    
    if (!badSearch)
        mainApp->CreateBrowserWindow(theHits);
    
    delete[] searchString;
    SetWatchCursor(FALSE);
}

//==================================================================================
//==================================================================================
//==================================================================================
                                     
