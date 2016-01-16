// *******************************************************************************************
// CExpertLogicWind.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************

#include "CExpertLogicWind.h"
#include "CExpertSearchWind.h"
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



// ---------------------------------------------------------------------------

CExpertLogicWind*
CExpertLogicWind::CreateLogicWindStream(
	LStream *inStream)
{
	return (NEW CExpertLogicWind(inStream));
}

// ---------------------------------------------------------------------------
//		¥ CExpertLogicWind
// ---------------------------------------------------------------------------
//	Default constructor
//
/*
CExpertLogicWind::CExpertLogicWind()
{
}
*/

// ---------------------------------------------------------------------------
//		¥ CExpertLogicWind
// ---------------------------------------------------------------------------
//	Construct-from-stream constructor.
//

CExpertLogicWind::CExpertLogicWind(LStream *inStream)
	: LWindow(inStream)
{
	mainApp = NULL;
}


// ---------------------------------------------------------------------------
//		¥ ~CExpertLogicWind
// ---------------------------------------------------------------------------
//	Destructor
//

CExpertLogicWind::~CExpertLogicWind()
{
}

//==================================================================================

void CExpertLogicWind::FinishCreateSelf()
{
	wordField = (CNeoEditField*) FindPaneByID(Logic_Field);
		ThrowIfNULL_(wordField);
	wordField->AddListener(this);
	SwitchTarget(wordField);

	proximityField = (LEditField*) FindPaneByID(Proximity_Box_Expert);
		ThrowIfNULL_(proximityField);
		
	LStdButton *theButton = (LStdButton*) FindPaneByID(Start_Expert_Logic_Search);
		ThrowIfNULL_( theButton );
	theButton->AddListener(this);

}

//==================================================================================
// ---------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------
//	Respond to clicks in the icon buttons
//

void
CExpertLogicWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
	long	value = *(long *)ioParam;
	int		i;
	
	switch (inMessage) {
		case Return_Key_Pressed:
		case Start_Expert_Logic_Search:
			try {
				DoSearch();
			} catch (CMemoryFullError) {
				SignalPStr_("\pMemory is full!  NeoSearch will attempt to continue but may"
							" not function correctly.");
			}
			break;

	}
}

//==================================================================================

void CExpertLogicWind::DoSearch() {

	CHitList		*theHits;
	char			*searchString;
	Str255			pSearchString;
	int				mode, prox;
	CTarget			theTarget;

	SetWatchCursor(TRUE);
	wordField->GetDescriptor(pSearchString);
	searchString = PascalToC(pSearchString);
	
	mode = CREATE_NEW;
	prox = proximityField->GetValue();
	theHits = NULL;

	Boolean done = FALSE;
	Boolean badSearch = FALSE;
	int		stringPos = 0;
	char	token[30];
	
	do {
		done = GetNextLowerCaseWord(token, searchString, stringPos);
		if (!strcmp(token, "and") || !strcmp(token, "&")) {
			mode = INTERSECT;
			continue;
		} else if (!strcmp(token, "or") || !strcmp(token, "|")) {
			mode = UNION; 
			continue;
		}

		if ((token[0] != '@') || (token[1] < '1') || (token[1] > '9'))
			return;					// invalid word typed, dump back & do nothing  FIX THIS

		CExpertSearchWind *theWind = mainApp->GetExpertSearchWindow(token[1] - '0');
		if (theWind == NULL) {
			badSearch = TRUE;
			break;				// **FIX THIS ** 
		}
		if (!(theWind->CreateTarget(theTarget))) {		// missing info?
			badSearch = TRUE;
			break;				// ***FIX THIS*** dump back if not valid, also dealloc mem
		}
		
		mainApp->giantTable->SetSearchMode(mode, theHits, prox);
		mainApp->giantTable->SetAmbiguityChecking(theTarget.GetAmbChecking());
		mainApp->giantTable->SetDisambiguationChecking(theTarget.GetDisambChecking());

		short searchType = theTarget.GetSearchType();
		if (searchType == STRING_SEARCH)
			theHits = mainApp->giantTable->FindHitsWithStringOf(theTarget);
		else if (searchType == SUBSTRING_SEARCH)
			theHits = mainApp->giantTable->FindHitsWithSubstringOf(theTarget);
		else if (searchType == BASE_SEARCH)
			theHits = mainApp->giantTable->FindHitsWithLemmaOf(theTarget);
		else
			theHits = mainApp->giantTable->FindHitsWithLemmaAndGrammarOf(theTarget);

		int 	error;
		Str255  pString;
		error = mainApp->giantTable->CheckForError();
		if (error == EXT_AMB_SEARCH_ERROR) {
			CToPascal(pString, theTarget.GetString());
			ParamText(pString,"\pit is externally ambiguous", NULL, NULL);
			StopAlert(Search_Nope_Dialog, NULL);
			badSearch = TRUE;
		} else if (error == NON_PLACE_SEARCH_ERROR) {
			CToPascal(pString, theTarget.GetString());
			ParamText(pString,"\pit has no grammatical meaning", NULL, NULL);
			StopAlert(Search_Nope_Dialog, NULL);
			badSearch = TRUE;
		} else if (error == CANCEL_SEARCH_ERROR) {
			badSearch = TRUE;
			break;
		}

		mode = INTERSECT; 	// in case of implicit AND (no conjunction given)

	} while (!done);
	
	if (!badSearch) 
		mainApp->CreateBrowserWindow(theHits);
	
	delete[] searchString;
	SetWatchCursor(FALSE);
}



//==================================================================================
									 
