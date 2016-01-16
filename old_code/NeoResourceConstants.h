// NeoResourceConstants.h

#pragma once


enum {Help_Intro, Help_Simple, Help_Expert, Help_Expert_Logic, Help_Hit_Browser,
	  Help_Direct_Browser, Help_Word_Inspector};


const	ResIDT	Window_Menu_ID = 133;

// Dialog Boxes
const	ResIDT	Error_Dialog = 129;
const	ResIDT	Search_Nope_Dialog = 130;
const	ResIDT	Startup_Dialog = 135;
//const	ResIDT	Progress_Dialog = 138;


// Powerplant Windows
const	ResIDT	Simple_Search_Window = 1000;
const   ResIDT	Expert_Search_Window = 2000;
const	ResIDT	Browser_Window = 3000;
const	ResIDT	Help_Window = 4000;
const	ResIDT	Expert_Logic_Window = 5000;
const	ResIDT	Direct_Browser_Window = 6000;
const	ResIDT	Word_Inspector_Dialog = 7000;

// Messages
const	MessageT	Open_Simple_Search_Command = 1000;
const	MessageT	Open_Expert_Search_Command = 2000;
//const	MessageT	Open_Browser_Command = 3000;
const	MessageT	Open_Help_Command = 4000;
const	MessageT	Help_Topic_List_Click = 4005;
const	MessageT	Open_Expert_Logic_Command = 5000;
const	MessageT	Open_Direct_Browser_Command = 6000;
const	MessageT	Browser_Book_List_Click = 3001;

const	MessageT	Start_Simple_Search = 1020;
const	MessageT	Start_Expert_Search = 2045;
const	MessageT	Start_Expert_Logic_Search = 5020;

// Simple Search Window Contents
const	ResIDT	Word_Edit_Simple = 1005;
const	ResIDT	Family_Check = 1010;
const	ResIDT	Substring_Check = 1011;
const	ResIDT	Proximity_Box_Simple = 1025;

// Expert Search Window Contents
const	ResIDT	Word_Edit_Expert = 2001;
const	ResIDT	Ambiguity_Check = 2010;
const	ResIDT	Disamb_Check = 2011;
const	ResIDT	String_Radio = 2020;
const	ResIDT	Substring_Radio = 2021;
const	ResIDT	Base_Radio = 2022;
const	ResIDT	BaseGrammar_Radio = 2023;
const	ResIDT	Grammar_Radio = 2024;
const	ResIDT	Case_List = 2030;
const	ResIDT	Gender_List = 2031;
const	ResIDT	Participle_List = 2032;
const	ResIDT	Mood_List = 2033;
const	ResIDT	Tense_List = 2034;
const	ResIDT	Voice_List = 2035;
const	ResIDT	Person_List = 2036;
const	ResIDT	Number_List = 2037;
const	ResIDT	Substantive_Radio = 2050;
const	ResIDT	Verb_Radio = 2051;

// Browser Window Contents
const	PaneIDT	Book_List = 3001;
const	PaneIDT	Scroller = 3002;
const	PaneIDT	Text_Field = 3003;
const	PaneIDT	Next_Button = 3020;
const	PaneIDT	Previous_Button = 3021;
const	PaneIDT	NextP_Button = 3022;
const	PaneIDT	PreviousP_Button = 3023;
const	PaneIDT	Direct_Next_Button = 3024;
const	PaneIDT	Direct_Previous_Button = 3025;
const	PaneIDT	Book_Title_Pane = 3030;
const	PaneIDT	Book_Number_Pane = 3035;
const	PaneIDT	Hit_Total_Pane = 3040;

// Help Window Contents
const	PaneIDT	Help_Topic_Pane = 4005;
const	PaneIDT	Help_Text_Pane = 4020;
const	ResIDT	Help_Resource_Base = 4200;

// Expert Logic Window Contents
const	PaneIDT	Logic_Field = 5005;
const	PaneIDT	Proximity_Box_Expert = 5025;

// Direct Browser Window Contents
const	PaneIDT	Direct_Book_List = 6005;
const	PaneIDT	Direct_Passage_Edit = 6010;
const	PaneIDT	Direct_Go = 6015;
const	PaneIDT	Direct_Scroller = 6002;
const	PaneIDT	Direct_Text_Field = 6003;
const	PaneIDT	Direct_Next_Button2 = 6024;
const	PaneIDT	Direct_Previous_Button2 = 6025;

// Word Inspector Dialog
const	PaneIDT	Inspected_Text = 7010;
const	PaneIDT	Dictionary_Lookup_Button = 7015;
const	PaneIDT	Meaning_List_Box = 7020;

