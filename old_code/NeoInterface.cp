// *******************************************************************************************
// NeoInterface.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************

#include "NeoInterface.h"

#include <LWindow.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <URegistrar.h>
#include <PPobClasses.h>
#include <LGrowZone.h>
#include <UMemoryMgr.h>
#include <UPowerTools.h>
#include <LStdControl.h>
#include <UDesktop.h>

// NEW for register all classes (this block is needed after replace RegisterAll)
//#include <LActiveScroller.h>
//#include <LButton.h>
#include <LCaption.h>
//#include <LCicnButton.h>
#include <LDialogBox.h>
#include <LEditField.h>
#include <LListBox.h>
//#include <LOffscreenView.h>
//#include <LPicture.h>
//#include <LPlaceHolder.h>
//#include <LPrintout.h>
#include <LScroller.h>
#include <LStdControl.h>
//#include <LTextEdit.h>
#include <LWindow.h>
#include <LRadioGroup.h>
//#include <LTabGroup.h>


#include "NeoResourceConstants.h"
#include "NeoException.h"
#include "CBrowserWind.h"
#include "CSimpleSearchWind.h"
#include "CExpertSearchWind.h"
#include "CExpertLogicWind.h"
#include "CDirectBrowserWind.h"
#include "CHelpWind.h"
#include "NeoUtils.h"
#include "CWASTEEdit.h"
#include "Menu_Utils.h"
#include "CNeoListBox.h"
#include "CNeoEditField.h"


const int Window_Menu_Command_Base = 500;		// synthetic commands for Window list on menu

//==================================================================================
//		¥ Main Program
//==================================================================================

void main(void)
{
									// Set Debugging options
#ifdef Debug_Throw
	gDebugThrow = debugAction_Alert;
#endif

#ifdef Debug_Signal
	gDebugSignal = debugAction_Alert;
#endif

	InitializeHeap(6);			// maybe need even more masters?
	InitializeToolbox();
	NEW LGrowZone(20000);
	NeoInterfaceApp	theApp;			

	Try_ {					// macro Try_/Catch_ loop catches interface problems
		try {				// ANSI try/catch loop catches internal engine errors
			theApp.Run();
		} catch (CFatalError engineError) {		// catch internal engine exceptions
			theApp.DisplayErrorDialog(engineError);
		}
	}
	Catch_ (err)						// If everything fails, we land here.
	{									// catch certain interface exceptions
		SignalPStr_( "\pAborting NeoSearch due to a fatal error.  (Sorry)" );
		ExitToShell();
	}
	EndCatch_;
//	DebugNewReportLeaks();		<-- moved to interfaceapp destructor
}


//==================================================================================
//	Constructor
//==================================================================================

NeoInterfaceApp::NeoInterfaceApp()
{
	// Register functions to create core PowerPlant classes
//	RegisterAllPPClasses();

	// all copied from RegisterAllPPClasses(), unneeded ones commented out
//	URegistrar::RegisterClass(LButton::class_ID,		LButton::CreateButtonStream);
	URegistrar::RegisterClass(LCaption::class_ID,		LCaption::CreateCaptionStream);
	URegistrar::RegisterClass(LDialogBox::class_ID,		LDialogBox::CreateDialogBoxStream);
	URegistrar::RegisterClass(LEditField::class_ID,		LEditField::CreateEditFieldStream);
	URegistrar::RegisterClass(LListBox::class_ID,		LListBox::CreateListBoxStream);
	URegistrar::RegisterClass(LPane::class_ID,			LPane::CreatePaneStream);
//	URegistrar::RegisterClass(LPicture::class_ID,		LPicture::CreatePictureStream);
//	URegistrar::RegisterClass(LPlaceHolder::class_ID,	LPlaceHolder::CreatePlaceHolderStream);
//	URegistrar::RegisterClass(LPrintout::class_ID,		LPrintout::CreatePrintoutStream);
//	URegistrar::RegisterClass('prto',					LPrintout::CreateOldPrintoutStream);
	URegistrar::RegisterClass(LScroller::class_ID,		LScroller::CreateScrollerStream);
	URegistrar::RegisterClass(LStdControl::class_ID,	LStdControl::CreateStdControlStream);
	URegistrar::RegisterClass(LStdButton::class_ID,		LStdButton::CreateStdButtonStream);
	URegistrar::RegisterClass(LStdCheckBox::class_ID,	LStdCheckBox::CreateStdCheckBoxStream);
	URegistrar::RegisterClass(LStdRadioButton::class_ID,LStdRadioButton::CreateStdRadioButtonStream);
	URegistrar::RegisterClass(LStdPopupMenu::class_ID,	LStdPopupMenu::CreateStdPopupMenuStream);
//	URegistrar::RegisterClass(LTextEdit::class_ID,		LTextEdit::CreateTextEditStream);
	URegistrar::RegisterClass(LView::class_ID,			LView::CreateViewStream);
	URegistrar::RegisterClass(LWindow::class_ID,		LWindow::CreateWindowStream);
	URegistrar::RegisterClass(LRadioGroup::class_ID,	LRadioGroup::CreateRadioGroupStream);
//	URegistrar::RegisterClass(LTabGroup::class_ID,		LTabGroup::CreateTabGroupStream);
	
	// Register custom classes
	URegistrar::RegisterClass('BrWi', CBrowserWind::CreateBrowserWindStream);
	URegistrar::RegisterClass('SSWi', CSimpleSearchWind::CreateSearchWindStream);
	URegistrar::RegisterClass('ESWi', CExpertSearchWind::CreateSearchWindStream);
	URegistrar::RegisterClass('ELWi', CExpertLogicWind::CreateLogicWindStream);
	URegistrar::RegisterClass('WAST', CWASTEEdit::CreateWASTEEditStream);
	URegistrar::RegisterClass('DiWi', CDirectBrowserWind::CreateDirectBrowserWindStream);
	URegistrar::RegisterClass('nlbx', CNeoListBox::CreateListBoxStream);
	URegistrar::RegisterClass('nefd', CNeoEditField::CreateEditFieldStream);
	URegistrar::RegisterClass('NHlp', CHelpWind::CreateHelpWindStream);
	

	// Display onscreen startup dialog box
	DialogPtr startupDialog = GetNewDialog(Startup_Dialog, NULL, 0);
	if (!startupDialog) {
		SignalPStr_("\pCan't load startup dialog resource!");
		ExitToShell();
	}
	DrawDialog(startupDialog);
	SetWatchCursor(TRUE);

	try {								// needed since not caught by App.Run()
		giantTable  = NEW CGiant;
		textFetcher = NEW CTextFetch;
	} catch (CFatalError f) {
		DisplayErrorDialog(f);
		ExitToShell();
//		Throw_ (1);
	}
	
	DisposeDialog(startupDialog);
	SetWatchCursor(FALSE);
	
//	expertSearchCount = 0;
	for (int j = 0; j < MAX_TARGETS; j++)
		expertSearchWindow[j] = NULL;
	helpWindow = NULL;
	windowListSize = 0;
}


//==================================================================================
//	Destructor
//==================================================================================

NeoInterfaceApp::~NeoInterfaceApp()
{
	delete textFetcher;
	delete giantTable;

//	DebugNewReportLeaks();	
}

//==================================================================================
//		¥ StartUp
//==================================================================================
//	This function lets you do something when the application starts up. 
//	For example, you could issue your own NEW command, or respond to a system
//  oDoc (open document) event.

void
NeoInterfaceApp::StartUp()
{
//	ObeyCommand(cmd_New, nil);		// example startup action
}

//==================================================================================
//		¥ ObeyCommand
//==================================================================================
//	Respond to commands

Boolean
NeoInterfaceApp::ObeyCommand(
	CommandT	inCommand,
	void		*ioParam)
{
	LWindow     *theWindow;
	LStdButton	*theButton;
	Boolean		cmdHandled = true;
	Str255		titleString;
	ResIDT		menuID;
	Int16		menuItem;

	if (IsSyntheticCommand(inCommand, menuID, menuItem)) {
		if (menuID == Window_Menu_ID) {		// someone selected a window from the window list

			if (menuItem > windowListSize) throw CFatalError("Menu Error");				
			LWindow *p = windowList[menuItem-1];
			if (p != NULL) {
				p->Show();
				p->Select();
			}
//				SwitchTarget(p);
//			WindowPtr p = UWindows::FindNthWindow(menuItem);
//			WindowPtr p = UWindows::FindNamedWindow(
//			if (p)
//				SwitchTarget(p);
	
		} else {
				//	Be sure to call the base class's ObeyCommand()
				//	member function to get its behavior.
			cmdHandled = LCommander::ObeyCommand(inCommand, ioParam);
//			needsRefresh = false;
		}
	} else switch (inCommand) {
//				Deal with command messages (defined in PP_Messages.h).
//				Any that you don't handle will be passed to the commander, LApplication

		case Open_Simple_Search_Command:
			CSimpleSearchWind *searchWindow = (CSimpleSearchWind*) 
									LWindow::CreateWindow(Simple_Search_Window, this);	
			ThrowIfNULL_(searchWindow);
			searchWindow->Setup(this);
//			searchWindow->Show();
			AddToOpenWindowList(searchWindow, "\pSimple Search Window");
			break;

		case Open_Expert_Search_Command:
			int j = 0;
			while ((j < MAX_TARGETS) && (expertSearchWindow[j] != NULL))
				j++;
			if (j == MAX_TARGETS) {
				SignalPStr_("\pSorry, too many expert search windows are open.");
				break;
			}

			CExpertSearchWind *expertWindow = (CExpertSearchWind*)
										LWindow::CreateWindow(Expert_Search_Window, this);	
			ThrowIfNULL_(expertWindow);
			expertWindow->GetDescriptor(titleString);
			AddNumberSuffix(j+1, titleString);
			expertWindow->SetDescriptor(titleString);
			expertWindow->Setup(this);
//			expertWindow->Show();
			expertSearchWindow[j] = expertWindow;
			AddToOpenWindowList(expertWindow, titleString);
			break;

		case Open_Expert_Logic_Command:
			CExpertLogicWind *logicWindow = (CExpertLogicWind*)
										LWindow::CreateWindow(Expert_Logic_Window, this);	
			ThrowIfNULL_(logicWindow);
			logicWindow->Setup(this);
//			logicWindow->Show();
			AddToOpenWindowList(logicWindow, "\pExpert Logic Window");
			break;

		case Open_Direct_Browser_Command:
			CDirectBrowserWind *directWindow = (CDirectBrowserWind*)
									LWindow::CreateWindow(Direct_Browser_Window, this);
			ThrowIfNULL_(directWindow);
			directWindow->Setup(this);
//			directWindow->Show();
			AddToOpenWindowList(directWindow, "\pDirect Browsing Window");
			break;

		case Open_Help_Command:
			OpenHelpWindow(Help_Intro);
		break;
		
		default:
			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);
			break;
	}
	
	return cmdHandled;
}

//==================================================================================
// Open a help window starting with topic # num

void NeoInterfaceApp::OpenHelpWindow(short num)
{
	if (helpWindow == NULL) {
		helpWindow = (CHelpWind*)	LWindow::CreateWindow(Help_Window, this);
		ThrowIfNULL_(helpWindow);
		helpWindow->LoadHelpTextTopic(num);
		helpWindow->Show();
		AddToOpenWindowList(helpWindow, "\pHelp Window");
	} else {
		helpWindow->LoadHelpTextTopic(num);
		helpWindow->Show();
		helpWindow->Select();
	}
}

//==================================================================================
//		¥ FindCommandStatus
//==================================================================================
//	This function enables menu commands.

void
NeoInterfaceApp::FindCommandStatus(
	CommandT	inCommand,
	Boolean		&outEnabled,
	Boolean		&outUsesMark,
	Char16		&outMark,
	Str255		outName)
{
	ResIDT	menuID;
	Int16	menuItem;

	if (IsSyntheticCommand(inCommand, menuID, menuItem)) {
		if (menuID == Window_Menu_ID) {

			if (windowListSize > 0) {
				short	j;
				short	frontWindowNumber = -1;

//				WindowPtr p = UWindows::FindNthWindow(1);		// front window
//				ThrowIfNULL_(p);
//				LWindow *lp = LWindow::FetchWindowObject(p);
				LWindow *lp = UDesktop::FetchTopRegular();
				ThrowIfNULL_(lp);
				for (j = 0; j < windowListSize; j++)
					if (windowList[j] == lp)
						frontWindowNumber = j;
				if (frontWindowNumber == -1) throw CFatalError("Front Window Not In List");
				
				UDynamicMenu::AdjustMenu(Window_Menu_ID, frontWindowNumber+1);
			} else
				UDynamicMenu::AdjustMenu(Window_Menu_ID, 0);

			outEnabled = true;
			outUsesMark = true;
			outMark = checkMark;

		} else 
				//	Be sure to call the base class's FindCommanStatus()
				//	member function to get its behavior
			LCommander::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);

	} else switch (inCommand) {
//				Return menu item status according to command messages.
//				Any that you don't handle will be passed to the commander, LApplication

		case Open_Simple_Search_Command:
		case Open_Expert_Search_Command:
		case Open_Expert_Logic_Command:
		case Open_Direct_Browser_Command:
		case Open_Help_Command:
			outEnabled = true;			// enable the New command
			outUsesMark = false;		// but it doesn't have a checkmark etc.
			break;
		
		case cmd_New:
			outEnabled = false;
			outUsesMark = false;
			break;
		
		default:
			LApplication::FindCommandStatus(inCommand, outEnabled,
												outUsesMark, outMark, outName);
			break;
	}
}


//==================================================================================

void NeoInterfaceApp::ListenToMessage(MessageT inMessage, void *ioParam)
{
//	Boolean	needsRefresh = true;

	switch (inMessage) {

//		default:
//			needsRefresh = false;
//			break;
	}
	
//	if (needsRefresh)
//		Refresh();
}

//==================================================================================

void NeoInterfaceApp::CreateBrowserWindow(CHitList *theHits) 
{
	CBrowserWind	*theWindow;

	theWindow = (CBrowserWind*) LWindow::CreateWindow(Browser_Window, this);	
	ThrowIfNULL_( theWindow );

	theWindow->Setup(theHits, this);
//	theWindow->Show();
//	LCommander *super = GetSuperCommander();
//	ThrowIfNULL_(super);
	AddToOpenWindowList(theWindow, "\pBrowsing Window");
}

//==================================================================================

void NeoInterfaceApp::DisplayErrorDialog(CFatalError& except)
{
	Str255	s1, s2;
	CToPascal(s1, except.Diagnostic());
	CToPascal(s2, except.WhatFailed());
	
	SignalPStr_( s1 );
//	ParamText(s1, s2, NULL, NULL);
//	StopAlert(Error_Dialog, NULL);
}

//==================================================================================

CExpertSearchWind* NeoInterfaceApp::GetExpertSearchWindow(short num)
{
	return expertSearchWindow[num-1];
}


//==================================================================================


void NeoInterfaceApp::WindowHasClosed(LWindow* window)
{
	if ((CHelpWind*) window == helpWindow)
		helpWindow = NULL;
	else
	for (int j = 0; j < MAX_TARGETS; j++)
		if (expertSearchWindow[j] == (CExpertSearchWind*) window) {
			expertSearchWindow[j] = NULL;
			break;
		}
}

//==================================================================================
// If a window can be closed, update the window list and menu to reflect this

Boolean NeoInterfaceApp::AllowSubRemoval(LCommander *inSub)
{
	Boolean result = LApplication::AllowSubRemoval(inSub);
	LWindow* closingWindow;

	if (result) {
		Str255	title;
		closingWindow = (LWindow*)inSub;
		WindowHasClosed(closingWindow);
		
		int j = 0;
		while ((windowList[j] != closingWindow) && (j < windowListSize))
			j++;
		if (j < windowListSize) {		// a registered window
	
	//		UDynamicMenu::RemoveItem(Window_Menu_ID, p->GetDescriptor(title));
			UDynamicMenu::RemoveItem(Window_Menu_ID, j+1, FALSE);
		
			windowListSize--;
			for (; j < windowListSize; j++)
				windowList[j] = windowList[j+1];
		}
	}
	return result;
}

//==================================================================================

void NeoInterfaceApp::AddToOpenWindowList(LWindow *windowPtr, Str255 windowTitle)
{
	UDynamicMenu::AppendItem(Window_Menu_ID, windowTitle, FALSE);
//		TRUE, 0, Window_Menu_Command_Base);
	if (windowListSize > MAX_OPEN_WINDOWS)
		throw CFatalError("Too many windows"); // fix this better (dynamic list)
	windowList[windowListSize++] = windowPtr;
}

//==================================================================================
// Open a word inspector dialog box for the given word

void NeoInterfaceApp::LaunchWordInspector(const char* word, LWindow* anchorWindow)
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

	
	int wordsFound = giantTable->InitializeGrammaticalDescriber(word);
	
	if (wordsFound > 0) {
		::LAddRow(wordsFound, 0, listMenu);
		while (giantTable->GetNextGrammaticalDescription(cTemp)) {
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

//==================================================================================

