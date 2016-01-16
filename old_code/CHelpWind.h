// CHelpWind.h

#pragma once

#include <LWindow.h>
#include <LListener.h>

#include "CWASTEEdit.h"
#include "CNeoListBox.h"



class	CHelpWind : public LWindow, public LListener {
public:
	static	CHelpWind *CreateHelpWindStream(LStream *inStream);
	
					CHelpWind(void);
					CHelpWind(LStream *inStream);
//	virtual			~CHelpWind(void);
	
//	void			Setup(CHitList*, NeoInterfaceApp*);
	void			ListenToMessage(MessageT inMessage, void *ioParam);
//	virtual void	DoClose();		// tell the application we're closing
	virtual void	LoadHelpTextTopic(short topicNumber);

protected:

//	NeoInterfaceApp	*mainApp;
	short			selected;
	short			totalTopics;

	CNeoListBox		*helpTopicPane;
	CWASTEEdit		*helpTextPane;
	
	virtual void	FinishCreateSelf();
};
