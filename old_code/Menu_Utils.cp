// ===========================================================================
// Menu_Utils.cp	  ©1995 J. Rodden, DD/MF & Associates. All rights reserved
// ===========================================================================

#include "Menu_Utils.h"

#include <LMenu.h>
#include <LMenuBar.h>
#include <String_Utils.h>
#include <LCommander.h>

// ---------------------------------------------------------------------------
//		¥ AppendItem
// ---------------------------------------------------------------------------

Int16
UDynamicMenu::AppendItem(
	Int16	inMENUid,
	Str255	inString,
	Boolean	inGenerateCommandNumbers,
	Int16	inSeparatorPosition,
	Int16	inBaseCommandNumber)
{
	LMenu*		theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMENUid);
	MenuHandle	menuH = theMenu->GetMacMenuH();
	Int16		menuLength = CountMItems(menuH);
	Str255		theMenuItem;
	
	// Insert separator line if neccessary
	if ( inSeparatorPosition > 0 && menuLength < inSeparatorPosition ) {
		CopyPStr("\p(-", theMenuItem);
	  ::InsertMenuItem( menuH, theMenuItem, ++menuLength);
	}
	
	// Prepare theMenuItem string
	CopyPStr(inString, theMenuItem);
	
	// Append inString to the menu
  ::InsertMenuItem( menuH, theMenuItem, ++menuLength);
	
	// Generate command number
	Int16 newCommandNumber = menuLength + inBaseCommandNumber - inSeparatorPosition - 1;
	if ( inGenerateCommandNumbers && newCommandNumber < 10 ) {
	  ::SetItemCmd( menuH, menuLength, '0' + newCommandNumber);
	}
	
	return menuLength;
}

// ---------------------------------------------------------------------------
//		¥ RemoveItem
// ---------------------------------------------------------------------------

Int16
UDynamicMenu::RemoveItem(
	Int16	inMENUid,
	Str255	inString,
	Boolean	inGenerateCommandNumbers,
	Int16	inSeparatorPosition,
	Int16	inBaseCommandNumber)
{
	LMenu*		theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMENUid);
	MenuHandle	menuH = theMenu->GetMacMenuH();
	Int16		menuLength = CountMItems(menuH);
	Str255		itemString;

	// Search for and remove inString from the menu
	for ( short i = inSeparatorPosition + 1; i <= menuLength; i++) {
	  ::GetItem( menuH, i, itemString);
		
		if( ::EqualString( itemString, inString, true, true)) {
		  ::DeleteMenuItem( menuH, i);
			menuLength--;
		}
		
		Int16 newCommandNumber = i + inBaseCommandNumber - inSeparatorPosition - 1;
		if ( inGenerateCommandNumbers && newCommandNumber < 10 ) {
		  ::SetItemCmd( menuH, i, '0' + newCommandNumber);
		}
	}
	
	// Remove separator line if neccessary
	if ( inSeparatorPosition > 0 && menuLength == inSeparatorPosition ) {
	  ::DeleteMenuItem( menuH, inSeparatorPosition);
		menuLength--;
	}

	return menuLength;
}
// ---------------------------------------------------------------------------
//		¥ RemoveItem
// ---------------------------------------------------------------------------

Int16
UDynamicMenu::RemoveItem(
	Int16	inMENUid,
	Int16	inNumber,
	Boolean	inGenerateCommandNumbers,
	Int16	inSeparatorPosition,
	Int16	inBaseCommandNumber)
{
	LMenu*		theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMENUid);
	MenuHandle	menuH = theMenu->GetMacMenuH();
	Int16		menuLength = CountMItems(menuH);
//	Str255		itemString;

	// Search for and remove inString from the menu
	for ( short i = inSeparatorPosition + 1; i <= menuLength; i++) {
//	  ::GetItem( menuH, i, itemString);
		
//		if( ::EqualString( itemString, inString, true, true)) {
		if (i == inNumber) {
		  ::DeleteMenuItem( menuH, i);
			menuLength--;
		}
		
		Int16 newCommandNumber = i + inBaseCommandNumber - inSeparatorPosition - 1;
		if ( inGenerateCommandNumbers && newCommandNumber < 10 ) {
		  ::SetItemCmd( menuH, i, '0' + newCommandNumber);
		}
	}
	
	// Remove separator line if neccessary
	if ( inSeparatorPosition > 0 && menuLength == inSeparatorPosition ) {
	  ::DeleteMenuItem( menuH, inSeparatorPosition);
		menuLength--;
	}

	return menuLength;
}

// ---------------------------------------------------------------------------
//		¥ AdjustMenu
// ---------------------------------------------------------------------------

void
UDynamicMenu::AdjustMenu(Int16 inMENUid, Int16 checkPosition)
{
	LMenu*		theMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMENUid);
	MenuHandle	menuH = theMenu->GetMacMenuH();
	Int16		itemCount = ::CountMItems(menuH);

	::EnableItem(menuH, 0);

	for (short i = 1; i <= itemCount; i++) {
		::EnableItem(menuH, i);
		::CheckItem(menuH, i, i == checkPosition);
	}
}


// ---------------------------------------------------------------------------
//		¥ IsSyntheticCommand
// ---------------------------------------------------------------------------
// Does a little more than LCommander's version. Gets you the string associated
// with the synthetic command which is usefull in window/view menus.

Boolean
IsSyntheticCommand(
	CommandT inCommand,
	Int16	 inMENUid,
	Int16	 outMenuItem,
	Str255	 outMenuString)
{
	Int16	menuID;
	
	if ( LCommander::IsSyntheticCommand( inCommand, menuID, outMenuItem)
		 && menuID == inMENUid ) {
		MenuHandle menuH = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMENUid)->GetMacMenuH();
	  ::GetItem( menuH, outMenuItem, outMenuString);
		return true;
	}
	
	return false;
}


// ---------------------------------------------------------------------------
//		¥ ToggleMenuItem
// ---------------------------------------------------------------------------
//	A utility routine to toggle menu items.
//	Taken from CSmpTxtCmdr.cp	©1994 Metrowerks Inc. All rights reserved.

void
ToggleMenuItem(
	CommandT inOldCommand,
	CommandT inNewCommand,
	Str255	 inNewCommandString)
{
	ResIDT		menuID;
	MenuHandle	menuH;
	Int16		menuItem;
	LMenuBar*	theMenuBar = LMenuBar::GetCurrentMenuBar();
	
	theMenuBar->FindMenuItem(inOldCommand, menuID, menuH, menuItem);

	if (menuItem != 0) {
		LMenu *theMenu = theMenuBar->FetchMenu(menuID);
		theMenu->SetCommand(menuItem, inNewCommand);
		SetMenuItemText(menuH, menuItem, inNewCommandString);
	}
}

