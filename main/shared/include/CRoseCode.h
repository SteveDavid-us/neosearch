// *******************************************************************************************
// CRoseCode.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Class for the code that holds the stem and ending information for Neo-Latin words, as
// designed by Jonathon Rose.
//
// Needs including by just about everything.
// *******************************************************************************************

// Unless you are one of the data preparation programs, you should never need to directly set
// a CRoseCode.  You should just be setting CTargets which can compare themselves to CRoseCodes.
// In the application CRoseCodes can only represent valid words that are already index, so in a 
// sense they are read only.  Targets are the settable search items but they can modify their
// contents (a CRoseCode) directly...

// Admittedly the information hiding in this class is not very good, and could use redesigning...

#ifndef _CROSECODE_H_
#define _CROSECODE_H_

// ###########################################################################################
//#include "Engine.h"

// ###########################################################################################

#define STEM_AMBIG						0xFFFF				// ambiguity indicators within RoseCodes
#define	HEX5_AMBIG						0xF
#define	HEX6_AMBIG						0xF

#define STEM_DONT_CARE					0xFFFE				// grammatical settings for target obj.
#define	HEX5_DONT_CARE					0xE
#define	HEX6_DONT_CARE					0xE
#define	HEX7_DONT_CARE					0xE
#define	HEX8_DONT_CARE					0xC

// ###########################################################################################
//#define ANY_GRAMMAR						-2;

const char hex5Name[][60] = {"","substantive stem","present stem","perfect active stem",
			"perfect passive pple stem","gerundive stem","present active pple stem",
			"future active pple stem","comparative of adverb or adjective",
			"superlative of adverb or adjective",
			"comparative of perfect passive pple stem",
			"superlative of perfect passive pple stem",
			"comparative of present active pple stem",
			"superlative of present active pple stem"};

enum {NO_STEM, SUBSTANTIVE_STEM, PRESENT_STEM, PERFECT_ACTIVE_STEM, PERFECT_PASSIVE_PPLE_STEM,
	  GERUNDIVE_STEM, PRESENT_ACTIVE_PPLE_STEM, FUTURE_ACTIVE_PPLE_STEM, COMPAR_ADV_ADJ_STEM,
	  SUPERL_ADV_ADJ_STEM, COMPAR_PERFECT_PASSIVE_PPLE_STEM, SUPERL_PERFECT_PASSIVE_PPLE_STEM,
	  COMPAR_PRESENT_ACTIVE_PPLE_STEM, SUPERL_PRESENT_ACTIVE_PPLE_STEM};

const char hex6Name[][30] = {"","nominative case","genitive case","dative case",
			"accusative case","ablative case","vocative case","locative case","indicative mood",
			"subjunctive mood","infinitive mood","imperative mood"};

enum {NO_CASE, NOMINATIVE_CASE, GENITIVE_CASE, DATIVE_CASE, ACCUSATIVE_CASE, ABLATIVE_CASE,
	  VOCATIVE_CASE, LOCATIVE_CASE};
enum {NO_MOOD, INDICATIVE_MOOD, SUBJUNCTIVE_MOOD, INFINITIVE_MOOD, IMPERATIVE_MOOD};
			
const char hex7Name[][30] = {"","","present active","present passive","imperfect active",
			"imperfect passive","future active","future passive","perfect active","",
			"pluperfect active","","future perfect active"};

enum {NO_TENSE, PRESENT_TENSE, IMPERFECT_TENSE, FUTURE_TENSE, PERFECT_TENSE,
	  PLUPERFECT_TENSE, FUTURE_PERFECT_TENSE};
enum {NO_VOICE, ACTIVE_VOICE, PASSIVE_VOICE};
			
const char hex8Name[][30] = {"","1st person singular","2nd person singular",
			"3rd person singular","","masculine singular","feminine singular","neuter singular",
			"plural","1st person plural","2nd person plural","3rd person plural","",
			"masculine plural","feminine plural","neuter plural"};

enum {NO_PERSON, FIRST_PERSON, SECOND_PERSON, THIRD_PERSON};
enum {NO_NUMBER, SINGULAR_NUMBER, PLURAL_NUMBER};
enum {NO_GENDER, MASCULINE_GENDER, FEMININE_GENDER, NEUTER_GENDER};						


// ###########################################################################################
typedef struct FourByteStruct {
	unsigned stem : 16;
	unsigned hex5 : 4;
	unsigned hex6 : 4;
	unsigned hex7 : 4;
	unsigned hex8 : 4;
};

// ###########################################################################################
class CRoseCode { 

	FourByteStruct		f; 
		
public:	
	
	friend 	class CTarget;									// Allow target object to directly
															// look at stem and hexes.

	unsigned short	GetStem(void)				{return f.stem;};
	
	Boolean	IsExtAmb(void)			{return (f.stem == STEM_AMBIG);};
	Boolean	IsMidAmb(void)			{return (f.hex5 == HEX5_AMBIG);};
	Boolean	IsIntAmb(void)			{return (f.hex6 == HEX6_AMBIG);};
	Boolean	IsAmb(void)				{return (IsExtAmb() || IsMidAmb() || IsIntAmb());};
											
	// These 3 functions return a short that is an address line # in an ambiguity table.
	// They then clear the address bytes so an unambiguous replacement may be ORed in.
	short StripToExtBase(void)		{short s = (((short)f.hex5)<<12) + (((short)f.hex6)<<8) +
											   (((short)f.hex7)<<4) + (short)f.hex8;
									 f.stem = f.hex5 = f.hex6 = f.hex7 = f.hex8 = 0;
									 return s; };
	short StripToMidBase(void)		{short s = (((short)f.hex6)<<8) +
											   (((short)f.hex7)<<4) + (short)f.hex8;
									 f.hex5 = f.hex6 = f.hex7 = f.hex8 = 0;
									 return s; };
	short StripToIntBase(void)		{short s = (((short)f.hex7)<<4) + (short)f.hex8;
									 f.hex6 = f.hex7 = f.hex8 = 0;
									 return s; };
	
	CRoseCode operator| (CRoseCode r) {CRoseCode z;			  z.f.stem = f.stem | r.f.stem;
										z.f.hex5=f.hex5|r.f.hex5; z.f.hex6=f.hex6|r.f.hex6;
										z.f.hex7=f.hex7|r.f.hex7; z.f.hex8=f.hex8|r.f.hex8;
										return z; };
										
	Boolean operator== (CRoseCode r) {return ((f.stem==r.f.stem) && (f.hex5==r.f.hex5)
										   && (f.hex6==r.f.hex6) && (f.hex7==r.f.hex7)
										   && (f.hex8==r.f.hex8)); };
											
	Boolean BLANK_ROSE_CODE(void)	{return ((f.stem==0xFFFF) && (f.hex5==0xF) && (f.hex6==0xF)
										   && (f.hex7==0xF) && (f.hex8==0xF)); };
										  
	void	SET_BLANK_ROSE_CODE(void)	{f.stem=0xFFFF; f.hex5=0xF; f.hex6=0xF;
													  f.hex7=0xF; f.hex8=0xF; }
													  
	// grammar descriptions
	const char*	DescribeGrammar(int num)	{const char* d;
											 if (num==1) d = hex5Name[f.hex5];
											 else if (num==2) d = hex6Name[f.hex6];
											 else if (num==3) d = hex7Name[f.hex7];
											 else if (num==4) d = hex8Name[f.hex8];
											 else d = "\0";
											 return d;
											 }

	short	GetArraySize(void)			{return f.stem;}; 	// These 4 are used for the information
	short	GetMaxReplacements(void)	{return f.hex5;}; 	// block on disk only
	void	SetArraySize(short a)		{f.stem = a;}; 		// (ambiguity table classes).
	void	SetMaxReplacements(short a)	{f.hex5 = a;}; 
													  
	// To be used for building tables and indexing ONLY:
	void	SetStem(unsigned short s) 	{f.stem = s;};
	void	SetHex5(short h)			{f.hex5 = h;};											
	void	SetHex6(short h)			{f.hex6 = h;};											
	void	SetHex7(short h)			{f.hex7 = h;};											
	void	SetHex8(short h)			{f.hex8 = h;};

    void EndianSwap() {
        f.stem = (f.stem>>8) | (f.stem<<8);
        unsigned char x;
        // fixes difference struct ordering
        // XXX - make this cleaner
        x = f.hex5; f.hex5 = f.hex6; f.hex6 = x;
        x = f.hex7; f.hex7 = f.hex8; f.hex8 = x;
    }

    static int CompareRoses(const void *r1, const void *r2) {
        FourByteStruct *fbs1 = &((CRoseCode*)r1)->f;
        FourByteStruct *fbs2 = &((CRoseCode*)r2)->f;
        if ( fbs1->stem != fbs2->stem ) return (int)fbs2->stem - (int)fbs1->stem;
        if ( fbs1->hex5 != fbs2->hex5 ) return (int)fbs2->hex5 - (int)fbs1->hex5;
        if ( fbs1->hex6 != fbs2->hex6 ) return (int)fbs2->hex6 - (int)fbs1->hex6;
        if ( fbs1->hex7 != fbs2->hex7 ) return (int)fbs2->hex7 - (int)fbs1->hex7;
        return (int)fbs2->hex8 - (int)fbs1->hex8;
    }

};
// ###########################################################################################

#endif // _CROSECODE_H_

