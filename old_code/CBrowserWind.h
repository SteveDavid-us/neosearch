// *******************************************************************************************
// CBrowserWind.h
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
#include "NeoInterface.h"
#include "CWASTEEdit.h"
#include "CNeoListBox.h"
#include "CTextExploder.h"
#include "CHitOffsetList.h"

const	short	NONE_SELECTED = -1;



class	CBrowserWind : public LWindow, public LListener {
public:
	static	CBrowserWind *CreateBrowserWindStream(LStream *inStream);
	
					CBrowserWind(void);
					CBrowserWind(LStream *inStream);
	virtual			~CBrowserWind(void);
	
	void			Setup(CHitList*, NeoInterfaceApp*);
	void			ListenToMessage(MessageT inMessage, void *ioParam);

protected:

	CHitList		*theHits;
	NeoInterfaceApp	*mainApp;
	long			totalHits;
	short			volumeCount;
	short			selected;
	long			*hitsPerVol;
	short			actualVolumeNum[MAX_VOLUMES];

	hit				currentHit;
	CNeoListBox		*bookListPane;
	LCaption		*bookTitlePane;
	LStdPopupMenu	*bookNumberPane;
	Boolean			popupWasClickedFlag;
	
	CTextExploder	textExploder;
	CHitOffsetList	hitOffsetList;
	
	Boolean			directAccessInProgress;

	virtual void	FinishCreateSelf();
	void			DisplayHit(hit theHit);
	void			DisplayTitles(short volNum);
	void 			SetPopupPassageListToNumber(short number);
	void			BuildHitOffsetList(hit theHit);

};
