// *******************************************************************************************
// CTarget.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Class to hold information about a search request.
//
// Created and set in the GUI, sent to GTOE search methods.
// *******************************************************************************************

// ###########################################################################################
#include "Types.h"
#include "CTarget.h"



CTarget::CTarget()
{
    SetStemDontCare();
    checkAmb = checkDisamb = TRUE;
    searchType = 0;
    ClearGrammar();
}
    
void CTarget::ClearGrammar()
{
    grammar.hex5 = HEX5_DONT_CARE;
    grammar.hex6 = HEX6_DONT_CARE;
    grammar.hex7 = HEX7_DONT_CARE;
    grammar.hex8 = HEX8_DONT_CARE;
//  SetParticiple(HEX5_DONT_CARE);
//  SetCaseMood(HEX6_DONT_CARE);
//  SetTenseVoice(HEX7_DONT_CARE);
//  SetPersonNumber(HEX8_DONT_CARE);
    tense = voice = number = person = gender = GRAMMAR_NOT_SET;
    hex7_complete = hex8_complete = TRUE;
    grammarActive = FALSE;
}

Boolean CTarget::DoesGrammarMatter(void)
{
    return grammarActive;
}

void CTarget::SetParticiple(short a)
{
    grammarActive = TRUE;
    grammar.hex5 = a;
}

void CTarget::SetCase(short a)
{
    grammarActive = TRUE;
    grammar.hex6 = a;
}

void CTarget::SetMood(short a)
{
    grammarActive = TRUE;
    if (a == NO_MOOD)
        grammar.hex6 = NO_MOOD;
    else grammar.hex6 = a + 7;
}

void CTarget::SetTense(short a)
{
    grammarActive = TRUE;
    tense = a;
    hex7_complete = FALSE;      // starts at true
    if (voice == NO_VOICE) {
        if (tense != NO_TENSE) {
            voice = GRAMMAR_NOT_SET;
            hex7_complete = FALSE;
        } else {
            grammar.hex7 = NO_TENSE;
            hex7_complete = TRUE;
        }
            
    } else if (voice != GRAMMAR_NOT_SET) {
        grammar.hex7 = tense*2 + voice-1;
        hex7_complete = TRUE;
    } else {
        grammar.hex7 = tense*2;
    }
}

void CTarget::SetVoice(short a)
{
    grammarActive = TRUE;
    voice = a;
    hex7_complete = FALSE;      // starts at true
    if (tense == NO_TENSE) {
        if (voice != NO_VOICE) {
            tense = GRAMMAR_NOT_SET;
            hex7_complete = FALSE;
        } else {
            grammar.hex7 = NO_VOICE;
            hex7_complete = TRUE;
        }
            
    } else if (tense != GRAMMAR_NOT_SET) {
        if (voice == NO_VOICE)
            grammar.hex7 = NO_VOICE;
        else
            grammar.hex7 = tense*2 + voice-1;
        hex7_complete = TRUE;
    } else {
        grammar.hex7 = voice-1;
    }
}

void CTarget::SetPerson(short a)
{
    grammarActive = TRUE;
    person = a;
    hex8_complete = FALSE;      // starts at true
    if (number == NO_NUMBER) {
        if (person != NO_PERSON) {
            number = GRAMMAR_NOT_SET;
            hex8_complete = FALSE;
        } else {
            grammar.hex8 = NO_PERSON;
            hex8_complete = TRUE;
        }
            
    } else if (number != GRAMMAR_NOT_SET) {
        grammar.hex8 = (number-1)*8 + person;
        hex8_complete = TRUE;
    } else {
        grammar.hex8 = person;
    }
}

void CTarget::SetGender(short a)
{
    grammarActive = TRUE;
    gender = a;
    hex8_complete = FALSE;      // starts at true
    if (number == NO_NUMBER) {
        if (gender != NO_GENDER) {
            number = GRAMMAR_NOT_SET;
            hex8_complete = FALSE;
        } else {
            grammar.hex8 = NO_GENDER;
            hex8_complete = TRUE;
        }
            
    } else if (number != GRAMMAR_NOT_SET) {
        if (gender == NO_GENDER)
            grammar.hex8 = (number-1)*8;
        else
            grammar.hex8 = (number-1)*8 + gender+4;
        hex8_complete = TRUE;
    } else {
        grammar.hex8 = gender+4;
    }
}

void CTarget::SetNumber(short a)
{
    grammarActive = TRUE;
    number = a;
    hex8_complete = FALSE;      // starts at true
    if ((person == NO_PERSON) || (gender == NO_GENDER)) {
        if (number != NO_NUMBER) {
            grammar.hex8 = (number-1)*8;
            hex8_complete = TRUE;
        } else {
            grammar.hex8 = 0;
            hex8_complete = TRUE;
        }
            
    } else if (person != GRAMMAR_NOT_SET) {
        if (number == NO_NUMBER)
            grammar.hex8 = 0;
        else
            grammar.hex8 = (number-1)*8 + person;
        hex8_complete = TRUE;
    } else if (gender != GRAMMAR_NOT_SET) {
        if (number == NO_NUMBER)
            grammar.hex8 = 0;
        else
            grammar.hex8 = (number-1)*8 + gender+4;
        hex8_complete = TRUE;
    } else {
        grammar.hex8 = (number-1)*8;
    }
}


inline Boolean CTarget::Hex7Matches(short hex)
{
    Boolean result = TRUE;
    if (tense != GRAMMAR_NOT_SET)
        result = (hex / 2 == tense);
    if (hex == NO_VOICE)            // fix for NO VOICE
        result = (hex == voice);
    else if (voice != GRAMMAR_NOT_SET)
        result = (hex % 2 + 1 == voice);    
    return result;
}


inline Boolean CTarget::Hex8Matches(short hex)
{
    Boolean result = TRUE;
    if (person != GRAMMAR_NOT_SET)
        result = (hex % 8 == person);
    if (gender != GRAMMAR_NOT_SET)
        result = (hex % 8 - 4 == gender);
    if (number != GRAMMAR_NOT_SET)
        result = (hex / 8 + 1 == number);
    return result;
}


Boolean CTarget::DoesMyGrammarMatchThis(CRoseCode r)
{
    return(
        ( (grammar.stem == STEM_DONT_CARE) || (grammar.stem == r.f.stem) ) &&
        ( (grammar.hex5 == HEX5_DONT_CARE) || (grammar.hex5 == r.f.hex5) ) &&
        ( (grammar.hex6 == HEX6_DONT_CARE) || (grammar.hex6 == r.f.hex6) ) &&
        (hex7_complete?  ((grammar.hex7 == HEX7_DONT_CARE) || (grammar.hex7 == r.f.hex7)) :
                        Hex7Matches(r.f.hex7) ) &&
        (hex8_complete?  ((grammar.hex8 == HEX8_DONT_CARE) || (grammar.hex8 == r.f.hex8)) :
                        Hex8Matches(r.f.hex8) ) 
                        
          );            
}

