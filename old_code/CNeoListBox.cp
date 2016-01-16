// ===========================================================================
//	LListBox.cp						©1993 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	A wrapper class for the Toolbox List Manager

#ifdef PowerPlant_PCH
#include PowerPlant_PCH
#endif


#include "CNeoListBox.h"


// ---------------------------------------------------------------------------
//		¥ CreateListBoxStream
// ---------------------------------------------------------------------------
//	Create a NEW ListBox object, initializing it with data from a Stream

CNeoListBox*
CNeoListBox::CreateListBoxStream(
	LStream	*inStream)
{
	return (NEW CNeoListBox(inStream));
}


// ---------------------------------------------------------------------------
//		¥ LListBox(LStream*)
// ---------------------------------------------------------------------------
//	Initialize a ListBox from the data in a Stream

CNeoListBox::CNeoListBox(
	LStream	*inStream)
		: LListBox(inStream)
{
}


// ---------------------------------------------------------------------------
//		¥ ~LListBox
// ---------------------------------------------------------------------------
//	Destructor
/*
LListBox::~LListBox()
{
	if (mMacListH != nil) {			// Dispose Toolbox ListHandle
		FocusDraw();
		::LDispose(mMacListH);
	}
}
*/


// ---------------------------------------------------------------------------
//		¥ ClickSelf
// ---------------------------------------------------------------------------
//	Respond to Click inside an ListBox

void
CNeoListBox::ClickSelf(
	const SMouseDownEvent	&inMouseDown)
{
	if (SwitchTarget(this)) {
		FocusDraw();
		
		::LClick(inMouseDown.whereLocal, inMouseDown.macEvent.modifiers, mMacListH);
					
		BroadcastMessage(mDoubleClickMessage, this);
		
	}
}

