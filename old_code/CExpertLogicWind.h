// *******************************************************************************************
// CExpertLogicWind.h
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

class	CExpertLogicWind : public LWindow, public LListener {
public:
	static	CExpertLogicWind *CreateLogicWindStream(LStream *inStream);
	
					CExpertLogicWind(void);
					CExpertLogicWind(LStream *inStream);
	virtual			~CExpertLogicWind(void);
	void			Setup(NeoInterfaceApp *app) {mainApp = app;};
	
	void			ListenToMessage(MessageT inMessage, void *ioParam);
//	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	

protected:

	NeoInterfaceApp	*mainApp;

	virtual void	FinishCreateSelf();
	void			DoSearch();
	
	CNeoEditField	*wordField;
	LEditField		*proximityField;
};
