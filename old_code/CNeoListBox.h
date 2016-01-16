// ===========================================================================
//	LListBox.h						©1993 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include <LListBox.h>

class CNeoListBox : public LListBox {
public:
	static CNeoListBox*	CreateListBoxStream(LStream *inStream);

					CNeoListBox(LStream *inStream);
//	virtual			~LListBox();
				

protected:
	virtual void	ClickSelf(const SMouseDownEvent &inMouseDown);

};
