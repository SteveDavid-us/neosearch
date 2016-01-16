
#pragma once

#include <LEditField.h>
#include <LBroadcaster.h>

const	MessageT	Return_Key_Pressed = 202;


class CNeoEditField : public LEditField, public LBroadcaster {
public:
	static CNeoEditField*	CreateEditFieldStream(LStream *inStream);

						CNeoEditField();
						CNeoEditField(LStream	*inStream);
	virtual				~CNeoEditField() {};
	
	virtual Boolean		HandleKeyPress(const EventRecord	&inKeyEvent);
	
};
