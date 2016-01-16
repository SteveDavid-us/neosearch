// *******************************************************************************************
// CExpertSearchWind.h					
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************


#pragma once


#include <LWindow.h>
#include <LListener.h>
#include <LCaption.h>
#include <LStdControl.h>

#include "NeoInterface.h"
#include "CTarget.h"
#include "CNeoEditField.h"


class	CExpertSearchWind : public LWindow, public LListener {
public:
	static	CExpertSearchWind *CreateSearchWindStream(LStream *inStream);
	
					CExpertSearchWind(void);
					CExpertSearchWind(LStream *inStream);
	virtual			~CExpertSearchWind(void);
	void			Setup(NeoInterfaceApp *app) {mainApp = app;};
	
	void			ListenToMessage(MessageT inMessage, void *ioParam);
//	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	
	
	Boolean			CreateTarget(CTarget& theTarget);
	

protected:

	NeoInterfaceApp	*mainApp;

	virtual void	FinishCreateSelf();
	void			DoExpertSearch();
	void			EnableCorrectGrammarBoxes(int setWhich);
	
	LStdRadioButton	*searchModeRadio[5];
	LStdRadioButton	*grammarTypeRadio[2];
	LStdCheckBox	*ambiguityCheckBox;
	LStdCheckBox	*disambiguationCheckBox;
	LStdPopupMenu	*grammarPopup[8];
	CNeoEditField	*wordField;
};
