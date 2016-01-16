// ===========================================================================
//	CNeoEditField.cp			   ©1993-1995 Metrowerks Inc. All rights reserved.
// ===========================================================================
//

#ifdef PowerPlant_PCH
#include PowerPlant_PCH
#endif


#include "CNeoEditField.h"

// ---------------------------------------------------------------------------
//		¥ CreateEditFieldStream
// ---------------------------------------------------------------------------
//	Create a new EditField object from the data in a Stream

CNeoEditField*
CNeoEditField::CreateEditFieldStream(
	LStream	*inStream)
{
	return (NEW CNeoEditField(inStream));
}


// ---------------------------------------------------------------------------
//		¥ CNeoEditField(LStream*)
// ---------------------------------------------------------------------------
//	Contruct an EditField from the data in a Stream

CNeoEditField::CNeoEditField(
	LStream	*inStream)
		: LEditField(inStream)
{
}


// ---------------------------------------------------------------------------
//		¥ ~CNeoEditField
// ---------------------------------------------------------------------------
//	Destructor
/*
CNeoEditField::~CNeoEditField()
{
	if (mTypingAction != nil) {
		PostAction(nil);
	}

	if (mTextEditH != nil) {
		::TEDispose(mTextEditH);
	}
}
*/


// ---------------------------------------------------------------------------
//		¥ SetKeyFilter
// ---------------------------------------------------------------------------
//	Specify the function for filtering keystrokes
/*
void
CNeoEditField::SetKeyFilter(
	KeyFilterFunc	inKeyFilter)
{
	mKeyFilter = inKeyFilter;
}
*/

// ---------------------------------------------------------------------------
//		¥ HandleKeyPress
// ---------------------------------------------------------------------------
//	Handle key stroke directed at an EditField
//
//	Return true if the EditField handles the keystroke

Boolean
CNeoEditField::HandleKeyPress(
	const EventRecord&	inKeyEvent)
{
	Int16		theKey = inKeyEvent.message & charCodeMask;
	if ((theKey == '\r') || (theKey == 3))
		BroadcastMessage(Return_Key_Pressed);
	return LEditField::HandleKeyPress(inKeyEvent);
}
