// *******************************************************************************************
// CExpertSearchWind.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************

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

CExpertSearchWind*
CExpertSearchWind::CreateSearchWindStream(
	LStream *inStream)
{
	return (NEW CExpertSearchWind(inStream));
}

// ---------------------------------------------------------------------------
//		¥ CExpertSearchWind
// ---------------------------------------------------------------------------
//	Default constructor
//
/*
CExpertSearchWind::CExpertSearchWind()
{
}
*/

// ---------------------------------------------------------------------------
//		¥ CExpertSearchWind
// ---------------------------------------------------------------------------
//	Construct-from-stream constructor.
//

CExpertSearchWind::CExpertSearchWind(LStream *inStream)
	: LWindow(inStream)
{
	mainApp = NULL;
}


// ---------------------------------------------------------------------------
//		¥ ~CExpertSearchWind
// ---------------------------------------------------------------------------
//	Destructor
//

CExpertSearchWind::~CExpertSearchWind()
{
}

//==================================================================================

void CExpertSearchWind::FinishCreateSelf() {
	int i;

	searchModeRadio[0] = (LStdRadioButton*) FindPaneByID(String_Radio);
	searchModeRadio[1] = (LStdRadioButton*) FindPaneByID(Substring_Radio);
	searchModeRadio[2] = (LStdRadioButton*) FindPaneByID(Base_Radio);
	searchModeRadio[3] = (LStdRadioButton*) FindPaneByID(BaseGrammar_Radio);
	searchModeRadio[4] = (LStdRadioButton*) FindPaneByID(Grammar_Radio);
	for (i = 0; i < 5; i++) {
		ThrowIfNULL_(searchModeRadio[i]);
		searchModeRadio[i]->AddListener(this);
	}
	
	grammarTypeRadio[0] = (LStdRadioButton*) FindPaneByID(Substantive_Radio);
	grammarTypeRadio[1] = (LStdRadioButton*) FindPaneByID(Verb_Radio);
	for (i = 0; i < 2; i++) {
		ThrowIfNULL_(grammarTypeRadio[i]);
		grammarTypeRadio[i]->AddListener(this);
	}
	
	ambiguityCheckBox = (LStdCheckBox*) FindPaneByID(Ambiguity_Check);
	ThrowIfNULL_(ambiguityCheckBox);
//	ambiguityCheckBox->AddListener(this);
	disambiguationCheckBox = (LStdCheckBox*) FindPaneByID(Disamb_Check);
	ThrowIfNULL_(disambiguationCheckBox);
//	disambiguationCheckBox->AddListener(this);

	grammarPopup[0] = (LStdPopupMenu*) FindPaneByID(Case_List);
	grammarPopup[1] = (LStdPopupMenu*) FindPaneByID(Gender_List);
	grammarPopup[2] = (LStdPopupMenu*) FindPaneByID(Participle_List);
	grammarPopup[3] = (LStdPopupMenu*) FindPaneByID(Mood_List);
	grammarPopup[4] = (LStdPopupMenu*) FindPaneByID(Tense_List);
	grammarPopup[5] = (LStdPopupMenu*) FindPaneByID(Voice_List);
	grammarPopup[6] = (LStdPopupMenu*) FindPaneByID(Person_List);
	grammarPopup[7] = (LStdPopupMenu*) FindPaneByID(Number_List);
	for (i = 0; i < 8; i++) {
		ThrowIfNULL_(grammarPopup[i]);
//		grammarPopup[i]->AddListener(this);	
	}

	wordField = (CNeoEditField*) FindPaneByID(Word_Edit_Expert);	ThrowIfNULL_(wordField);
	ThrowIfNULL_(wordField);
	wordField->AddListener(this);
	SwitchTarget(wordField);
	
	ambiguityCheckBox->Disable();			// This block is for default option graying-out
	disambiguationCheckBox->Disable();		// (string search)
	wordField->Enable();
	for (i = 0; i < 2; i++)
		grammarTypeRadio[i]->Disable();
	for (i = 0; i < 8; i++)
		grammarPopup[i]->Disable();	
		
	LStdButton *theButton = (LStdButton*) FindPaneByID(Start_Expert_Search);
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
CExpertSearchWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
	long	value = *(long *)ioParam;
	int		i;
	
	switch (inMessage) {
		case Return_Key_Pressed:
		case Start_Expert_Search:
			try {
				DoExpertSearch();
			} catch (CMemoryFullError) {
				SignalPStr_("\pMemory is full!  NeoSearch will attempt to continue but may"
							" not function correctly.");
			}
			break;
		case String_Radio:
		case Substring_Radio:
			if (value) {
				ambiguityCheckBox->Disable();
				disambiguationCheckBox->Disable();
				wordField->Enable();
				for (i = 0; i < 2; i++)
					grammarTypeRadio[i]->Disable();
				for (i = 0; i < 8; i++)
					grammarPopup[i]->Disable();	
			}
			break;
		case Base_Radio:
			if (value) {
				ambiguityCheckBox->Enable();
				disambiguationCheckBox->Enable();
				wordField->Enable();
				for (i = 0; i < 2; i++)
					grammarTypeRadio[i]->Disable();
				for (i = 0; i < 8; i++)
					grammarPopup[i]->Disable();	
			}
			break;
		case BaseGrammar_Radio:
			if (value) {
				ambiguityCheckBox->Enable();
				disambiguationCheckBox->Enable();
				wordField->Enable();
				for (i = 0; i < 2; i++)
					grammarTypeRadio[i]->Enable();
				EnableCorrectGrammarBoxes(grammarTypeRadio[1]->GetValue() ? 1 : 0);
			}
			break;
		case Grammar_Radio:
			if (value) {
				ambiguityCheckBox->Enable();
				disambiguationCheckBox->Enable();
				wordField->Disable();
				for (i = 0; i < 2; i++)
					grammarTypeRadio[i]->Enable();
				EnableCorrectGrammarBoxes(grammarTypeRadio[1]->GetValue() ? 1 : 0);
			}
			break;
		case Substantive_Radio:
			if (value)
				EnableCorrectGrammarBoxes(0);
			break;
		case Verb_Radio:
			if (value)
				EnableCorrectGrammarBoxes(1);
			break;
	}
}

//==================================================================================
void CExpertSearchWind::EnableCorrectGrammarBoxes(int setWhich)
{
	int i;
	if (setWhich == 0) {
		for (i = 0; i < 3; i++)
			grammarPopup[i]->Enable();
		for (i = 3; i < 7; i++)
			grammarPopup[i]->Disable();
	} else {
		for (i = 0; i < 3; i++)
			grammarPopup[i]->Disable();
		for (i = 3; i < 7; i++)
			grammarPopup[i]->Enable();
	}
	grammarPopup[7]->Enable();
}

//==================================================================================
/*
void CExpertSearchWind::DoClose() {
	mainApp->WindowHasClosed(this);
	LWindow::DoClose();
}
*/
//void CExpertSearchWind::AttemptClose() {
//	mainApp->WindowHasClosed(this);
//	LWindow::AttemptClose();
//}

//==================================================================================

Boolean CExpertSearchWind::CreateTarget(CTarget& theTarget)
{
	Str255			pSearchString;
	char			*searchString = NULL;
	short			searchType;
	int				caseG, genderG, participleG, moodG, tenseG, voiceG, personG, numberG;
	int				whichGrammar;

	int i = 0;
	while (!(searchModeRadio[i]->GetValue()))
		i++;
	searchType = i;
	
	theTarget.SetSearchType(searchType);

	if (searchType < GRAMMAR_SEARCH) {
		wordField->GetDescriptor(pSearchString);
		searchString = PascalToC(pSearchString);

		int loop;
		for (loop = 0; (searchString[loop]==' ') || (searchString[loop]=='\t'); loop++);
		if (searchString[loop]=='\0') {
			delete[] searchString;
			return FALSE;
		}
			
		theTarget.SetString(searchString);
	} else
		theTarget.SetStemDontCare();
		
	theTarget.SetAmbChecking( ambiguityCheckBox->GetValue() );
	theTarget.SetDisambChecking( disambiguationCheckBox->GetValue() );

	if (searchType > BASE_SEARCH) {		
		whichGrammar = (grammarTypeRadio[0]->GetValue()? 0 : 1);
		numberG = grammarPopup[7]->GetValue();
		
		if (whichGrammar == 0) {		// substantive
			caseG = grammarPopup[0]->GetValue();
			genderG = grammarPopup[1]->GetValue();
			participleG = grammarPopup[2]->GetValue();
			
			if (participleG != 1)		// default is don't care
				theTarget.SetParticiple(participleG - 2);
			if (caseG != 1)
				theTarget.SetCaseMood(caseG - 2);
			if (((genderG == 1) && (numberG > 1)) || ((genderG > 1) && (numberG == 1)) ||
			    ((genderG > 2) && (numberG == 2))) {
			    	SignalPStr_("\pInvalid Gender/Number Combination!");
			    	if (searchString) delete[] searchString;
			    	return FALSE;
		    }
			if ((genderG == 2) && (numberG == 2))
				theTarget.SetPersonNumber(0);
			else if ((genderG != 1) && (numberG != 1))
				theTarget.SetPersonNumber((numberG-3)*8 + (genderG==2? 0 : genderG+2));
							
		} else {						// verb
			moodG = grammarPopup[3]->GetValue();
			tenseG = grammarPopup[4]->GetValue();
			voiceG = grammarPopup[5]->GetValue();
			personG = grammarPopup[6]->GetValue();
			
			if (moodG != 1)
				theTarget.SetCaseMood( moodG == 2 ? 0 : moodG + 5 );
			if (((tenseG == 1) && (voiceG > 1)) || ((tenseG > 1) && (voiceG == 1)) ||
			    ((tenseG == 2) && (voiceG > 2)) || ((tenseG > 2) && (voiceG == 2))) {
			    	SignalPStr_("\pInvalid Tense/Voice Combination!");
			    	if (searchString) delete[] searchString;
			    	return FALSE;
		    } 
			if ((tenseG == 2) && (voiceG == 2))
				theTarget.SetTenseVoice(0);
			else if ((tenseG != 1) && (voiceG != 1))
				theTarget.SetTenseVoice( (tenseG-2)*2 + voiceG-3 );

			if (((personG == 1) && (numberG > 1)) || ((personG > 1) && (numberG == 1)) ||
			    ((personG > 2) && (numberG == 2))) {
			    	SignalPStr_("\pInvalid Person/Number Combination!");
			    	if (searchString) delete[] searchString;
			    	return FALSE;
		    }
			if ((personG == 2) && (numberG == 2))
				theTarget.SetPersonNumber(0);
			else if ((personG != 1) && (numberG != 1))
				theTarget.SetPersonNumber((numberG-3)*8 + personG-2);

		}
	}

	if (searchString)
		delete[] searchString;
	return TRUE;   // no info missing
}

//==================================================================================
void CExpertSearchWind::DoExpertSearch() {

	CHitList		*theHits = NULL;
	CTarget			theTarget;

	SetWatchCursor(TRUE);
	if (!CreateTarget(theTarget))		// <<---- error checking needed!
		return;
		
	mainApp->giantTable->SetSearchMode(CREATE_NEW, theHits, 0);
	mainApp->giantTable->SetAmbiguityChecking( theTarget.GetAmbChecking() );
	mainApp->giantTable->SetDisambiguationChecking( theTarget.GetDisambChecking() );

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
		return;
	} else if (error == NON_PLACE_SEARCH_ERROR) {
		CToPascal(pString, theTarget.GetString());
		ParamText(pString,"\pit has no grammatical meaning", NULL, NULL);
		StopAlert(Search_Nope_Dialog, NULL);
		return;
	} else if (error == CANCEL_SEARCH_ERROR)
		return;

	mainApp->CreateBrowserWindow(theHits);
	SetWatchCursor(FALSE);
}


//==================================================================================
//==================================================================================
//==================================================================================
									 
