// *******************************************************************************************
// CDirectBrowserWind.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************


#include "CDirectBrowserWind.h"
#include "NeoResourceConstants.h"
#include "NeoException.h"
#include "IndexerCodes.h"
#include "NeoUtils.h"
#include "CWordInspector.h"
#include <LStream.h>
#include <LStdControl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



// ---------------------------------------------------------------------------
//		¥ CreateTracerPaneStream [static]
// ---------------------------------------------------------------------------
//	This is the function you register with URegistrar to create a
//	CreateTracerPaneStream pane from a resource

CDirectBrowserWind*
CDirectBrowserWind::CreateDirectBrowserWindStream(
	LStream *inStream)
{
	return (NEW CDirectBrowserWind(inStream));
}


// ---------------------------------------------------------------------------
//		¥ CDirectBrowserWind
// ---------------------------------------------------------------------------
//	Construct-from-stream constructor.
//

CDirectBrowserWind::CDirectBrowserWind(LStream *inStream)
	: LWindow(inStream)
{
//	theHits = NULL;
	mainApp = NULL;
//	totalHits = 0;
	volumeCount = 0;
	selected = NO_DIRECT_BOOK_SELECTED;
//	hitsPerVol = NULL;
//	for (int j = 0; j < MAX_VOLUMES; j++)
//		actualVolumeNum[j] = -1;
	bookListPane = NULL;
//	bookTitlePane = NULL;
	bookNumberPane = NULL;
}


// ---------------------------------------------------------------------------
//		¥ ~CDirectBrowserWind
// ---------------------------------------------------------------------------
//	Destructor
//

CDirectBrowserWind::~CDirectBrowserWind()
{
//	if (theHits)
//		delete theHits;
}

//==================================================================================

void CDirectBrowserWind::FinishCreateSelf() {
	LStdButton	*button;
	
	button = (LStdButton*) FindPaneByID(Direct_Next_Button2);	ThrowIfNULL_( button );
	button->AddListener(this);
	button = (LStdButton*) FindPaneByID(Direct_Previous_Button2);	ThrowIfNULL_( button );
	button->AddListener(this);
	button = (LStdButton*) FindPaneByID(Direct_Go);	ThrowIfNULL_( button );
	button->AddListener(this);
	
	bookListPane = (LStdPopupMenu*) FindPaneByID(Direct_Book_List);
	ThrowIfNULL_( bookListPane );
	bookListPane->AddListener(this);
	bookNumberPane = (CNeoEditField*) FindPaneByID(Direct_Passage_Edit);
	ThrowIfNULL_( bookNumberPane );
	bookNumberPane->AddListener(this);
	
}

//==================================================================================

void CDirectBrowserWind::Setup(NeoInterfaceApp *app) {

	char	text[150];
	char	temp[20];
//	theHits = newHits;
	mainApp = app;

//	if (theHits == NULL)
//		return;

	CWASTEEdit *textPane = (CWASTEEdit*)FindPaneByID(Direct_Text_Field);  
	ThrowIfNULL_(textPane);
	textPane->AddListener(this);
	textExploder.Setup(app->textFetcher, app->giantTable, textPane, &hitOffsetList);
		
//	totalHits = theHits->ReportTotalHits();
//	volumeCount = theHits->ReportNumberOfVolumes();
//	hitsPerVol = theHits->ReportHitsPerVol();
	volumeCount = MAX_VOLUMES;
	
	
	MenuHandle	titleMenu;
	char		*bookName;
	Str255		pBookName;
	
	titleMenu = bookListPane->GetMacMenuH();
	for (short j = 0; j < volumeCount; j++) {
		bookName = mainApp->textFetcher->GetBookName(j);
		CToPascal(pBookName, bookName);
		::AppendMenu(titleMenu, pBookName);
	}
	bookListPane->SetMaxValue(volumeCount);
	bookListPane->SetValue(1);
	bookListPane->Draw(NULL);
}


// ---------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------
//	Respond to clicks in the icon buttons
//

void
CDirectBrowserWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
	long	value; // = *(long *)ioParam;
	hit		where;

	switch (inMessage) {
		case Direct_Book_List:
/*			Cell tempCell;
			Boolean found = bookListPane->GetLastSelectedCell(tempCell);
			if (!found) break;
			value = tempCell.v;
			if (value != selected) {
				selected = value;
				theHits->ResetVolume(actualVolumeNum[selected]);
				DisplayTitles(actualVolumeNum[selected]);
				
				currentHit.passage = -1;  // flags number popup listener that it wasn't user select
				if (theHits->ReportNextHit(where))
					DisplayHit(where);
			}	
*/		
			break;
		
		case Direct_Passage_Edit:
/*			long tempL;
			if (popupWasClickedFlag == FALSE) break;
			StringToNum((StringPtr)ioParam, &tempL);
			where.passage = tempL;
			tempL = bookNumberPane->GetValue();
			theHits->ResetVolume(actualVolumeNum[selected]);
			for (int i = 0; i < tempL; i++)
				if (! theHits->ReportNextPassageHit(where) )
					SignalPStr_("\pFinding passage failed");
			DisplayHit(where);
*/
			break;
		
		case Return_Key_Pressed:
		case Direct_Go:
			where.volume = bookListPane->GetValue() - 1;
			where.passage = bookNumberPane->GetValue();
			selected = where.volume;
			if ((where.passage <= mainApp->textFetcher->GetHighestPassageNumber(where.volume)) &&
			    (where.passage >= mainApp->textFetcher->GetLowestPassageNumber(where.volume))) 
				DisplayHit(where);
			else
				SignalPStr_("\pPassage Number out of Range");
		
			break;
		
		case Direct_Next_Button2:
			if (selected != NO_DIRECT_BOOK_SELECTED) {
				where = currentHit;
				if (where.passage < mainApp->textFetcher->GetHighestPassageNumber(where.volume)) {
					where.passage++;	// error check needed
//					where.word = 0;
//					directAccessInProgress = TRUE;
					DisplayHit(where);
					bookNumberPane->SetValue(where.passage);
				}
			}
			break;

		case Direct_Previous_Button2:
			if (selected != NO_DIRECT_BOOK_SELECTED) {
				where = currentHit;
				if (where.passage > mainApp->textFetcher->GetLowestPassageNumber(where.volume)) {
					where.passage--;	// error check needed
//					where.word = 0;
//					directAccessInProgress = TRUE;
					DisplayHit(where);
					bookNumberPane->SetValue(where.passage);
				}
			}
			break;

		case Command_Dbl_Click_Message:
			mainApp->LaunchWordInspector((char*)ioParam, this);
			delete[] (char*)ioParam;
			break;

	}
}

//==================================================================================


void CDirectBrowserWind::DisplayHit(hit theHit) {
//	long		hitStart, hitEnd;
//	static hit	lastRealHit;		// (not from direct access)
	
//	if (directAccessInProgress || (theHit.volume != currentHit.volume) ||
//							 (theHit.passage != currentHit.passage)) {

//		if (theHit.word != 0) {				// if not direct access
		
//	SetPopupPassageListToNumber(theHit.passage);
//	BuildHitOffsetList(theHit);
//	directAccessInProgress = FALSE; // ensure a passage is reloaded if direct access ended

//		} else
//			hitOffsetList.Reset();

//		if ((theHit.word == 0) && (theHit.passage == lastRealHit.passage)) {
//			BuildHitOffsetList(lastRealHit);
//		}
	SetWatchCursor(TRUE);
	textExploder.LoadPassageNumber(theHit);
	textExploder.FeedPassageToWASTE(0, 0);
	SetWatchCursor(FALSE);
//	}
	
//	if (theHit.word != 0) {
//		CWASTEEdit *textPane = (CWASTEEdit*)FindPaneByID(Text_Field);  
//		ThrowIfNULL_(textPane);

//		hitStart = hitOffsetList.GetStartOffset(theHit.word);
//		hitEnd = hitOffsetList.GetEndOffset(theHit.word);
//		textPane->SetSelection(hitStart, hitEnd);
//		textExploder.HighlightHit(theHit.word);
//		SwitchTarget(textPane);		 // makes highlighting visible
//		lastRealHit = theHit;
//	}
	currentHit = theHit;

}


// ###########################################################################################
//==================================================================================
/*
void CDirectBrowserWind::LaunchWordInspector(Handle theText, long start, long end) {
	// replace this stuff with launching of the window:
	char buffer[80];
	Str255 buf2;
	long i;
	for (i = start; i < end; i++)
		buffer[i-start] = (*theText)[i];
	buffer[i-start] = '\0';
	CToPascal(buf2, buffer);
	SignalPStr_(buf2);
}
*/
//==================================================================================

