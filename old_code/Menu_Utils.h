// ===========================================================================
// Menu_Utils.h		  ©1995 J. Rodden, DD/MF & Associates. All rights reserved
// ===========================================================================

#pragma once

class UDynamicMenu {
public:
	static Int16	AppendItem(Int16 inMENUid, Str255 inString,
								Boolean inGenerateCommandNumbers = true,
								Int16 inSeparatorPosition = 0,
								Int16 inBaseCommandNumber = 1);

	static Int16	RemoveItem(Int16 inMENUid, Str255 inString,
								Boolean inGenerateCommandNumbers = true,
								Int16 inSeparatorPosition = 0,
								Int16 inBaseCommandNumber = 1);

	static Int16	RemoveItem(Int16 inMENUid, Int16 inNumber,
								Boolean inGenerateCommandNumbers = true,
								Int16 inSeparatorPosition = 0,
								Int16 inBaseCommandNumber = 1);

	static void	AdjustMenu(Int16 inMENUid, Int16 checkPosition);
};

Boolean	IsSyntheticCommand(CommandT inCommand, Int16 inMENUid,
							Int16 outMenuItem, Str255 outMenuItemString);

void	ToggleMenuItem(CommandT inOldCommand, CommandT inNewCommand,
							Str255 inNewCommandString);
