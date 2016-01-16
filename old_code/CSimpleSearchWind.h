// *******************************************************************************************
// CSimpleSearchWind.h					
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************


#pragma once

#include <LWindow.h>
#include <LListener.h>
#include <LCaption.h>
#include <LStdControl.h>
#include <LEditField.h>
#include "NeoInterface.h"
#include "CNeoEditField.h"
//class NeoInterfaceApp;


class	CSimpleSearchWind : public LWindow, public LListener {
public:
	static	CSimpleSearchWind* CreateSearchWindStream(LStream *inStream);
	
					CSimpleSearchWind(void);
					CSimpleSearchWind(LStream *inStream);
	virtual			~CSimpleSearchWind(void);
	void			Setup(NeoInterfaceApp *app) {mainApp = app;};
	void			ListenToMessage(MessageT inMessage, void *ioParam);
//	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	

protected:

//	virtual void	DrawSelf(void);
	virtual void	FinishCreateSelf();
	void			DoSimpleSearch();
	
	NeoInterfaceApp	*mainApp;
	LStdCheckBox	*familyCheckBox;
	LStdCheckBox	*substringCheckBox;
	CNeoEditField	*wordField;
	LEditField		*proximityField;
};
