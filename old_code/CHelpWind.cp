// ===========================================================================
//	CHelpWind.cp					©1994 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "CHelpWind.h"
#include "NeoResourceConstants.h"
#include "NeoException.h"
#include "NeoUtils.h"
#include <LStream.h>
#include <LListBox.h>
#include <string.h>


// ---------------------------------------------------------------------------
//		¥ CreateTracerPaneStream [static]
// ---------------------------------------------------------------------------
//	This is the function you register with URegistrar to create a
//	CreateTracerPaneStream pane from a resource

CHelpWind*
CHelpWind::CreateHelpWindStream(
	LStream *inStream)
{
	return (NEW CHelpWind(inStream));
}


// ---------------------------------------------------------------------------
//		¥ CHelpWind
// ---------------------------------------------------------------------------
//	Construct-from-stream constructor.
//

CHelpWind::CHelpWind(LStream *inStream)
	: LWindow(inStream)
{
	helpTopicPane = NULL;
	helpTextPane = NULL;
	selected = 0;
}


// ---------------------------------------------------------------------------
//		¥ ~CHelpWind
// ---------------------------------------------------------------------------
//	Destructor
//
/*
CHelpWind::~CHelpWind()
{
}
*/

//==================================================================================

void CHelpWind::FinishCreateSelf() {
	
	helpTopicPane = (CNeoListBox*) FindPaneByID(Help_Topic_Pane); ThrowIfNULL_( helpTopicPane );
	helpTopicPane->AddListener(this);
	
	helpTextPane = (CWASTEEdit*) FindPaneByID(Help_Text_Pane); ThrowIfNULL_( helpTextPane );	
//	LoadHelpTextTopic(selected);
	
	ListHandle theList = helpTopicPane->GetMacListH();
	StringHandle	newText;
	Cell			theCell;
	char*			cString;
	
	theCell.h = 0;
	::LDoDraw(FALSE, theList);
	::LAddColumn(1, 0, theList);
	
	totalTopics = 0;
	do {
		newText = GetString(Help_Resource_Base + totalTopics);
		if (newText) {
			cString = PascalToC(*newText);
			::LAddRow(1, totalTopics, theList);
			theCell.v = totalTopics;
			::LSetCell(cString, strlen(cString), theCell, theList);

			totalTopics++;
			delete[] cString;
		}
	} while (newText != NULL);
	::LDoDraw(TRUE, theList);

}



// ---------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------
//	Respond to clicks in the icon buttons
//

void
CHelpWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
	long	value; // = *(long *)ioParam;

	switch (inMessage) {
		case Help_Topic_List_Click:
			Cell tempCell;
			Boolean found = helpTopicPane->GetLastSelectedCell(tempCell);
			if (!found) break;
			value = tempCell.v;
			if (value != selected) {
				selected = value;
				LoadHelpTextTopic(selected);
			}	
		break;
	}
}

//==================================================================================
/*
void CHelpWind::Setup(CHitList *newHits, NeoInterfaceApp *app) {

	char	text[150];
	char	temp[20];
	theHits = newHits;
	mainApp = app;

	if (theHits == NULL)
		return;

	CWASTEEdit *textPane = (CWASTEEdit*)FindPaneByID(Text_Field);  
	ThrowIfNULL_(textPane);
	textExploder.Setup(app, textPane, &hitOffsetList);
		
	totalHits = theHits->ReportTotalHits();
	volumeCount = theHits->ReportNumberOfVolumes();
	hitsPerVol = theHits->ReportHitsPerVol();

	// Initialize data in list box cells
	LListBox *theListBox = (LListBox*)FindPaneByID(Book_List);		ThrowIfNULL_( theListBox );
	ListHandle theList = theListBox->GetMacListH();
	LAddColumn(1, 0, theList);
	LAddRow(volumeCount, 0, theList);
	Cell	theCell;
	int		outLineNum = 0;
	theCell.h = 0;

	for (int j = 0; j < MAX_VOLUMES; j++) {
		if (hitsPerVol[j] > 0) {
			theCell.v = outLineNum;
			strcpy(text, mainApp->textFetcher->GetBookName(j));
			sprintf(temp, " [%d]", hitsPerVol[j]);
			strcat(text, temp);
			LSetCell(text, strlen(text), theCell, theList);
			actualVolumeNum[outLineNum] = j;
			outLineNum++;
		}
	}
	
	LCaption *totalHitsCaption = (LCaption*)FindPaneByID(Hit_Total_Pane); 
		ThrowIfNULL_(totalHitsCaption);
	Str255 tempPString;
	char   tempCString[50];
	sprintf(tempCString, "TOTAL HITS: %d", totalHits);
	CToPascal(tempPString, tempCString);
	totalHitsCaption->SetDescriptor(tempPString);
//	totalHitsCaption->Draw(NULL);
}
*/
//==================================================================================

void CHelpWind::LoadHelpTextTopic(short topicNumber)
{
	Handle	newText = GetResource('TEXT', Help_Resource_Base + topicNumber);
	ThrowIfNULL_(newText);
	
	helpTextPane->InhibitDrawing();
	helpTextPane->Insert(newText, GetHandleSize(newText), NULL, NULL, TRUE);
	helpTextPane->SetSelection(0,0);
	helpTextPane->AllowDrawing();

}

//==================================================================================
/*
void CHelpWind::DoClose()
{
	NeoInterfaceApp* app = (NeoInterfaceApp*) GetSuperCommander();
	ThrowIfNULL_(app);
	
	app->WindowHasClosed(this);
	LWindow::DoClose();
}
*/
