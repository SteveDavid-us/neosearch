// *******************************************************************************************
// IndexerCodes.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Contains constant codes to aid in retaining meaning in the indexed text.
//
// This file should be included by the GUI text-exploder module and by the
// indexer program ONLY.
// *******************************************************************************************

#ifndef _INDEXERCODES_H_
#define _INDEXERCODES_H_
// 			*** MASK CODES ***
// Masks, when ANDed with a code, produce a flag for whether a condition exists...
// bits 0-16 are already reserved for our table index range (0->130000)

// ORDER OF MASK EVALUATION:
//   26 and 28 should be checked first for pre-output
//   Either 18 or 19 may apply to the word
//   21, 22, 23, 24, or 25 may apply
//   27 and 29 can apply even after punctuation
//   20 indicates that a space will follow everything
//   30 comes at the very end

const long 	M_CAPITAL=		1 << 18;	// bit 18
const long	M_ALL_CAPS=		1 << 19;
const long	M_SPACE=		1 << 20;
const long	M_COMMA=		1 << 21;
const long	M_PERIOD=		1 << 22;
const long	M_COLON=		1 << 23;
const long	M_SEMI_COLON=	1 << 24;
const long	M_QUESTIONM=	1 << 25;
const long	M_L_QUOTE=		1 << 26;
const long	M_R_QUOTE=		1 << 27;
const long	M_L_PAREN=		1 << 28;
const long	M_R_PAREN=		1 << 29;
const long	M_NEWLINE=		1 << 30;
//const long				1 << 31;	// 31 may not be safe to use (signed/unsigned
										// problems).


//			*** SPECIAL INDEX CODES ***
//  Special index codes should be checked for before a word is decoded -- these
//  will fail if you try to look them up in the GTOE.

const long I_BASE=			120000;

const long I_MISSINGWORD=	I_BASE-2;	// Says the indexer couldn't find it
const long I_BLANKWORD=		I_BASE-1;	// Says don't look up this word but strip its punctuation
const long I_PASSAGE_NUM=	I_BASE+0;	// A Swedenborg passage # follows
const long I_SUB_PASSAGE=	I_BASE+1;	// A Swedenborg sub-passage # follows
const long I_TITLE_START=	I_BASE+2;	// A chapter title begins here
const long I_TITLE_END=		I_BASE+3;	// A chapter title ends here
const long I_FOOTNOTE_REF=	I_BASE+4;	// A footnote reference follows
const long I_FOOT_START=	I_BASE+5;	// A footnote itself starts
const long I_FOOT_END=		I_BASE+6;	// A footnote itself ends
const long I_BIBLE_START=	I_BASE+7;	// Bible excerpt begins
const long I_BIBLE_END=		I_BASE+8;	// Bible excerpt ends
const long I_TEXT_START=	I_BASE+9;	// Straight text characters follow
const long I_TEXT_END=		I_BASE+10;	// End of straight text
const long I_NEW_LINE=		I_BASE+11;	// Jump to the next line (RETURN CODE)
const long I_TAB=			I_BASE+12;	// Indent for the beginning of a line
const long I_L_SQUAREB=		I_BASE+14;	// left square bracket
const long I_R_SQUAREB=		I_BASE+15;	// right square bracket
const long I_DASH=			I_BASE+16;	// dash (-)
const long I_ASTERIX=		I_BASE+17;	// * (replace this when figured out meaning)
const long I_EQUALSIGN=		I_BASE+18;	// equal sign (=)

										// STAIRS footnote stuff
										// Swedenborg excerpt (other volume) begins
										// Swedenborg excerpt ends

#endif // _INDEXERCODES_H_

