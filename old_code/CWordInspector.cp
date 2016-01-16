// CWordInspector.cp


#include "CWordInspector.h"
#include "NeoResourceConstants.h"
#include "NeoUtils.h"
#include <LDialogBox.h>
#include <LListBox.h>
#include <LEditField.h>


void CWordInspector::LaunchWordInspector(const char* word, LWindow* anchorWindow)
{
	Str255		pTemp;
	char		cTemp[255];
	ListHandle	listMenu;
	Cell		theCell;
	int			listSize = 0;
	
//	StDialogHandler	theHandler(Word_Inspector_Dialog, this);
//	LWindow			*theDialog = theHandler.GetDialog();
	
	LDialogBox *theDialog = (LDialogBox *) LWindow::CreateWindow(Word_Inspector_Dialog, anchorWindow);
	ThrowIfNULL_(theDialog);
	LEditField *wordField = (LEditField *)theDialog->FindPaneByID(Inspected_Text);
	ThrowIfNULL_(wordField);
	LListBox	*meaningList = (LListBox*) theDialog->FindPaneByID(Meaning_List_Box);
	ThrowIfNULL_(meaningList);
	
	listMenu = meaningList->GetMacListH();
	::LDoDraw(FALSE, listMenu);
	::LAddColumn(1, 0, listMenu);
	theCell.h = 0;

	// put the data in the correct fields
	int		i;
//	for (i = start; i < end; i++)
//		cTemp[i-start] = tolower((*theText)[i]);
//	cTemp[i-start] = '\0';
	CToPascal(pTemp, word);
	wordField->SetDescriptor(pTemp);

	
	int wordsFound = mainApp->giantTable->InitializeGrammaticalDescriber(word);
	
	if (wordsFound > 0) {
		::LAddRow(wordsFound, 0, listMenu);
		while (mainApp->giantTable->GetNextGrammaticalDescription(cTemp)) {
			theCell.v = listSize;
			::LSetCell(cTemp, strlen(cTemp), theCell, listMenu);
			listSize++;
		}
	} else {
		char NOT_FOUND_STRING[] = "SORRY, THIS WORD WAS NOT FOUND";
		theCell.v = 0;
		::LAddRow(1, 0, listMenu);
		::LSetCell(NOT_FOUND_STRING, strlen(NOT_FOUND_STRING), theCell, listMenu);
	}
	
	::LDoDraw(TRUE, listMenu);
//	theDialog->SetLatentSub(wordField);
	theDialog->Show();
}

