// *******************************************************************************************
// CDirectBrowserWind.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************


#pragma once

#include <LWindow.h>
#include <LListener.h>
#include <LCaption.h>
#include <LStdControl.h>

#include "CHitList.h"
#include "CTextFetch.h"
#include "CNeoEditField.h"
#include "NeoInterface.h"
#include "CWASTEEdit.h"
#include "CHitOffsetList.h"
#include "CTextExploder.h"

const int NO_DIRECT_BOOK_SELECTED = -1;

class	CDirectBrowserWind : public LWindow, public LListener {
public:
	static	CDirectBrowserWind *CreateDirectBrowserWindStream(LStream *inStream);
	
//					CDirectBrowserWind(void);
					CDirectBrowserWind(LStream *inStream);
	virtual			~CDirectBrowserWind(void);
	
	void			Setup(NeoInterfaceApp*);
	void			ListenToMessage(MessageT inMessage, void *ioParam);
//	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	

protected:

	NeoInterfaceApp	*mainApp;
	short			volumeCount;
	short			selected;

	hit				currentHit;
	LStdPopupMenu	*bookListPane;
	CNeoEditField	*bookNumberPane;
	
	CTextExploder	textExploder;
	CHitOffsetList	hitOffsetList;	// just a dummy empty class for the text exploder

	virtual void	FinishCreateSelf();
	void			DisplayHit(hit theHit);	

};

