// *******************************************************************************************
// NeoInterface.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************

#pragma once

#include <LApplication.h>
#include <LListener.h>

#include "Engine.h"
#include "CGiant.h"
#include "CHitList.h"
#include "CTextFetch.h"
#include "CTarget.h"
#include "IndexerCodes.h"
#include "NeoException.h"

class CHelpWind;
class CExpertSearchWind;
class LWindow;


const short MAX_TARGETS = 9;		// constrains # of expert search windows open at once
const short MAX_OPEN_WINDOWS = 60;	// maximum of all types of windows


class	NeoInterfaceApp : public LApplication, public LListener {
public:
					NeoInterfaceApp();		// constructor registers all PPobs
	virtual			~NeoInterfaceApp();		// stub destructor
	
		// this overriding function performs application functions
	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	
	
		// this overriding function returns the status of menu items
	virtual void	FindCommandStatus(CommandT inCommand,
						Boolean &outEnabled, Boolean &outUsesMark,
						Char16 &outMark, Str255 outName);
	virtual Boolean		AllowSubRemoval(LCommander *inSub);	// override to update window list
	virtual void		ListenToMessage(MessageT inMessage, void *ioParam);

	void				DisplayErrorDialog(CFatalError&);
	void				CreateBrowserWindow(CHitList*);
	CExpertSearchWind* 	GetExpertSearchWindow(short num);
	void 				OpenHelpWindow(short num);
	void				LaunchWordInspector(const char* word, LWindow* anchorWindow);

	CGiant				*giantTable;		// giant table of equivalents
	CTextFetch			*textFetcher;		// text fetching class to read Writings from disk

protected:

	CExpertSearchWind	*expertSearchWindow[MAX_TARGETS];		// list of expert windows open
	CHelpWind			*helpWindow;							// only one help window can be open
	LWindow				*windowList[MAX_OPEN_WINDOWS];			// list of all windows open
	short				windowListSize;

	virtual void	StartUp();				// overriding startup functions
	void			WindowHasClosed(LWindow* window);			// called internally to clean up window list
	void			AddToOpenWindowList(LWindow *windowPtr, Str255 windowTitle);
};
