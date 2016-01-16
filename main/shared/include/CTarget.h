// *******************************************************************************************
// CTarget.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Class to hold information about a search request.
//
// Created and set in the GUI, sent to GTOE search methods.
// *******************************************************************************************

#ifndef _CTARGET_H_
#define _CTARGET_H_

// ###########################################################################################
#include "Engine.h"
#include "CRoseCode.h"
#include <string.h>


enum {STRING_SEARCH, SUBSTRING_SEARCH, BASE_SEARCH, BASEGRAMMAR_SEARCH, GRAMMAR_SEARCH};

const short GRAMMAR_NOT_SET = -1;


// ###########################################################################################
class CTarget { 
protected:

	FourByteStruct	grammar;
    Boolean         grammarActive;
	short			tense, voice, number, person, gender;	// store individual values
	Boolean			hex7_complete, hex8_complete;
	char			string[101];				// max size of an edit field
	Boolean			checkAmb, checkDisamb;
	short			searchType;

	Boolean			Hex7Matches(short hex);
	Boolean			Hex8Matches(short hex);
		
public:	
	
	CTarget();// {SetStemDontCare(); checkAmb = checkDisamb = TRUE; searchType = 0;
/*				SetParticiple(HEX5_DONT_CARE);  SetCaseMood(HEX6_DONT_CARE);
				SetTenseVoice(HEX7_DONT_CARE);  SetPersonNumber(HEX8_DONT_CARE);
				tense = voice = number = person = gender = GRAMMAR_NOT_SET;
				hex7_complete = hex8_complete = FALSE;};
*/	
	// METHODS FOR THE GTOE TO CALL
	char*	GetString(void)					{return string; };
	Boolean	DoesStemMatter(void)			{return string[0] != '\0'; };
	Boolean	DoesGrammarMatter(void);
	void	SetStemValue(unsigned short v)	{grammar.stem = v; };
	short	GetSearchType()					{return searchType;};
	Boolean	GetAmbChecking()				{return checkAmb;};
	Boolean	GetDisambChecking()				{return checkDisamb;};

	// METHODS FOR THE GUI TO CALL
    void    SetGrammarDoCare(void)          {grammarActive = TRUE; };
	void 	ClearGrammar();
	void	SetString(const char *theString)		{strcpy(string, theString); };
	void	SetStemDontCare(void)			{string[0] = '\0'; grammar.stem = STEM_DONT_CARE;};
	void	SetSearchType(short s)			{searchType = s;};
	void	SetAmbChecking(Boolean a)		{checkAmb = a;};
	void	SetDisambChecking(Boolean d)	{checkDisamb = d;};
			//  These first four are temporary for the text interface
//	void	SetParticiple(short a)			{grammar.hex5 = a; };
//	void	SetCaseMood(short a)			{grammar.hex6 = a; };
//	void	SetTenseVoice(short a)			{grammar.hex7 = a; };
//	void	SetPersonNumber(short a)		{grammar.hex8 = a; };

			// These will be used by the graphical interface:
			// Need to define constants to send to them.
	void	SetParticiple(short a);//	{grammar.hex5 = a; };
	void	SetCase(short a);//		{grammar.hex6 = a; };
	void	SetMood(short a);//		{if (a == NO_MOOD) grammar.hex6 = NO_MOOD;
								//	 else grammar.hex6 = a + 7;};
	void	SetTense(short a);//		{	grammar.hex7 = a; };
	void	SetVoice(short a);//		{grammar.hex7 = a; };
	void	SetPerson(short a);//		{grammar.hex8 = a; };
	void	SetGender(short a);
	void	SetNumber(short a);//		{grammar.hex8 = a; };


	Boolean DoesMyGrammarMatchThis(CRoseCode r);
/*		{	return	( (grammar.stem == STEM_DONT_CARE) || (grammar.stem == r.f.stem) ) &&
					( (grammar.hex5 == HEX5_DONT_CARE) || (grammar.hex5 == r.f.hex5) ) &&
					( (grammar.hex6 == HEX6_DONT_CARE) || (grammar.hex6 == r.f.hex6) ) &&
					( (grammar.hex7 == HEX7_DONT_CARE) || (grammar.hex7 == r.f.hex7) ) &&
					( (grammar.hex8 == HEX8_DONT_CARE) || (grammar.hex8 == r.f.hex8) ); 	};
*/
};
// ###########################################################################################

#endif // _CTARGET_H_

