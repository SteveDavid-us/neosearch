// *******************************************************************************************
// CBrowserWind.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1995 -- HBL
// *******************************************************************************************

#include "CBrowserWind.h"
#include "NeoResourceConstants.h"
#include "NeoException.h"
#include "IndexerCodes.h"
#include "NeoUtils.h"
#include "CWordInspector.h"

#include <LStream.h>
#include <LListBox.h>
#include <LStdControl.h>
#include <LDialogBox.h>
#include <LEditField.h>
#include <LCaption.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//==================================================================================
//==================================================================================


// ---------------------------------------------------------------------------

CBrowserWind* CBrowserWind::CreateBrowserWindStream(LStream *inStream)
{
    return (NEW CBrowserWind(inStream));
}

// ---------------------------------------------------------------------------
//      ¥ CBrowserWind
// ---------------------------------------------------------------------------
//  Default constructor
//
/*
CBrowserWind::CBrowserWind()
{
}
*/

// ---------------------------------------------------------------------------
//      ¥ CBrowserWind
// ---------------------------------------------------------------------------
//  Construct-from-stream constructor.
//

CBrowserWind::CBrowserWind(LStream *inStream)
    : LWindow(inStream)
{
    theHits = NULL;
    mainApp = NULL;
    totalHits = 0;
    volumeCount = 0;
    selected = NONE_SELECTED;
    hitsPerVol = NULL;
    for (int j = 0; j < MAX_VOLUMES; j++)
        actualVolumeNum[j] = -1;
    bookListPane = NULL;
    bookTitlePane = NULL;
    bookNumberPane = NULL;
}


// ---------------------------------------------------------------------------
//      ¥ ~CBrowserWind
// ---------------------------------------------------------------------------
//  Destructor
//

CBrowserWind::~CBrowserWind()
{
    if (theHits)            // this class owns the hit list & must erase it when done
        delete theHits;
}

//==================================================================================

void CBrowserWind::FinishCreateSelf()
{
    LStdButton  *button;
    CNeoListBox *listBox;
    
    listBox = (CNeoListBox*) FindPaneByID(Book_List);       ThrowIfNULL_( listBox );
    listBox->AddListener(this);
    bookListPane = listBox;
    
    button = (LStdButton*) FindPaneByID(Next_Button);       ThrowIfNULL_( button );
    button->AddListener(this);
    button = (LStdButton*) FindPaneByID(Previous_Button);   ThrowIfNULL_( button );
    button->AddListener(this);
    button = (LStdButton*) FindPaneByID(NextP_Button);      ThrowIfNULL_( button );
    button->AddListener(this);
    button = (LStdButton*) FindPaneByID(PreviousP_Button);  ThrowIfNULL_( button );
    button->AddListener(this);
    button = (LStdButton*) FindPaneByID(Direct_Next_Button);    ThrowIfNULL_( button );
    button->AddListener(this);
    button = (LStdButton*) FindPaneByID(Direct_Previous_Button);    ThrowIfNULL_( button );
    button->AddListener(this);
    
    bookTitlePane = (LCaption*) FindPaneByID(Book_Title_Pane);
    ThrowIfNULL_( bookTitlePane );
    bookNumberPane = (LStdPopupMenu*) FindPaneByID(Book_Number_Pane);
    ThrowIfNULL_( bookNumberPane );
    bookNumberPane->AddListener(this);
}

//==================================================================================
// ---------------------------------------------------------------------------
//      ¥ ListenToMessage
// ---------------------------------------------------------------------------
//  Respond to clicks in the icon buttons
//

void
CBrowserWind::ListenToMessage(MessageT inMessage, void* ioParam)
{
    long    value; // = *(long *)ioParam;
    hit     where;

    switch (inMessage) {
        case Browser_Book_List_Click:
            Cell tempCell;
            Boolean found = bookListPane->GetLastSelectedCell(tempCell);
            if (!found) break;
            value = tempCell.v;
            if (value != selected) {
                selected = value;
                SetWatchCursor(TRUE);
                theHits->ResetVolume(actualVolumeNum[selected]);
                DisplayTitles(actualVolumeNum[selected]);
                SetWatchCursor(FALSE);
                currentHit.passage = -1;  // flags number popup listener that it wasn't user select
                if (theHits->ReportNextHit(where))
                    DisplayHit(where);
            }   
            break;
        
        case Book_Number_Pane:
            long tempL;
            if (popupWasClickedFlag == FALSE) break;
            StringToNum((StringPtr)ioParam, &tempL);
            where.passage = tempL;
            tempL = bookNumberPane->GetValue();
            theHits->ResetVolume(actualVolumeNum[selected]);
            for (int i = 0; i < tempL; i++)
                if (! theHits->ReportNextPassageHit(where) )
                    SignalPStr_("\pFinding passage failed");
            DisplayHit(where);
            break;
        
        case Next_Button:
            if (selected != NONE_SELECTED)
                if (theHits->ReportNextHit(where))
                    DisplayHit(where);
            break;

        case Previous_Button:
            if (selected != NONE_SELECTED)
                if (theHits->ReportPreviousHit(where))
                    DisplayHit(where);
            break;

        case NextP_Button:
            if (selected != NONE_SELECTED)
                if (theHits->ReportNextPassageHit(where))
                    DisplayHit(where);
            break;

        case PreviousP_Button:
            if (selected != NONE_SELECTED)
                if (theHits->ReportPreviousPassageHit(where))
                    DisplayHit(where);
            break;

        case Direct_Next_Button:
            if (selected != NONE_SELECTED) {
                where = currentHit;
                if (where.passage < mainApp->textFetcher->GetHighestPassageNumber(where.volume)) {
                    where.passage++;    
                    where.word = 0;
                    directAccessInProgress = TRUE;
                    DisplayHit(where);
                }
            }
            break;

        case Direct_Previous_Button:
            if (selected != NONE_SELECTED) {
                where = currentHit;
                if (where.passage > mainApp->textFetcher->GetLowestPassageNumber(where.volume)) {
                    where.passage--;    
                    where.word = 0;
                    directAccessInProgress = TRUE;
                    DisplayHit(where);
                }
            }
            break;
        
        case Command_Dbl_Click_Message:
            mainApp->LaunchWordInspector((char*)ioParam, this);
            delete[] (char*)ioParam;
            break;
    }
}

//==================================================================================

void CBrowserWind::Setup(CHitList *newHits, NeoInterfaceApp *app)
{
    char    text[150];
    char    temp[20];
    theHits = newHits;
    mainApp = app;

    if (theHits == NULL)
        return;

    CWASTEEdit *textPane = (CWASTEEdit*)FindPaneByID(Text_Field);  
    ThrowIfNULL_(textPane);
    textPane->AddListener(this);        // for command-dbl click message
    textExploder.Setup(app->textFetcher, app->giantTable, textPane, &hitOffsetList);
        
    totalHits = theHits->ReportTotalHits();
    volumeCount = theHits->ReportNumberOfVolumes();
    hitsPerVol = theHits->ReportHitsPerVol();

    // Initialize data in list box cells
    CNeoListBox *theListBox = (CNeoListBox*)FindPaneByID(Book_List);    ThrowIfNULL_( theListBox );
    ListHandle theList = theListBox->GetMacListH();
    ::LDoDraw(FALSE, theList);
    ::LAddColumn(1, 0, theList);
    ::LAddRow(volumeCount, 0, theList);
    Cell    theCell;
    int     outLineNum = 0;
    theCell.h = 0;

    for (int j = 0; j < MAX_VOLUMES; j++) {
        if (hitsPerVol[j] > 0) {
            theCell.v = outLineNum;
            strcpy(text, mainApp->textFetcher->GetBookName(j));
            sprintf(temp, " [%d]", hitsPerVol[j]);
            strcat(text, temp);
            ::LSetCell(text, strlen(text), theCell, theList);
            actualVolumeNum[outLineNum] = j;
            outLineNum++;
        }
    }
    ::LDoDraw(TRUE, theList);
    
    LCaption *totalHitsCaption = (LCaption*)FindPaneByID(Hit_Total_Pane); 
        ThrowIfNULL_(totalHitsCaption);
    Str255 tempPString;
    char   tempCString[50];
    sprintf(tempCString, "TOTAL HITS: %d", totalHits);
    CToPascal(tempPString, tempCString);
    totalHitsCaption->SetDescriptor(tempPString);
//  totalHitsCaption->Draw(NULL);
}

//==================================================================================

void CBrowserWind::DisplayHit(hit theHit)
{
    long        hitStart, hitEnd;
    static hit  lastRealHit;        // (not from direct access)
    Boolean     mustLoadNewPassage;
    
    mustLoadNewPassage = (directAccessInProgress || (theHit.volume != currentHit.volume) ||
                         (theHit.passage != currentHit.passage));

    if (mustLoadNewPassage) {
        SetWatchCursor(TRUE);
        if (theHit.word != 0) {             // if not direct access
            SetPopupPassageListToNumber(theHit.passage);
            BuildHitOffsetList(theHit);
            directAccessInProgress = FALSE; // ensure a passage is reloaded if direct access ended
        } else
            hitOffsetList.Reset();

        if ((theHit.word == 0) && (theHit.passage == lastRealHit.passage)) {
            BuildHitOffsetList(lastRealHit);    // highlight again if you say DIRECT N, DIRECT P.
        }

        textExploder.LoadPassageNumber(theHit);

        if (theHit.word != 0) {             // not direct access
            hitStart = hitOffsetList.GetStartOffset(theHit.word);
            hitEnd = hitOffsetList.GetEndOffset(theHit.word);
        } else
            hitStart = hitEnd = 0;
        textExploder.FeedPassageToWASTE(hitStart, hitEnd);  // highlighting now avoids ugly redraw

        SetWatchCursor(FALSE);
    }
    
    if (theHit.word != 0) {
        CWASTEEdit *textPane = (CWASTEEdit*)FindPaneByID(Text_Field);  
        ThrowIfNULL_(textPane);

        if (!mustLoadNewPassage) {      // we haven't yet set the highlighting

            hitStart = hitOffsetList.GetStartOffset(theHit.word);
            hitEnd = hitOffsetList.GetEndOffset(theHit.word);
            textPane->SetSelection(hitStart, hitEnd);
//          textExploder.HighlightHit(theHit.word);
        }
        SwitchTarget(textPane);      // makes highlighting visible
        lastRealHit = theHit;
    }
    currentHit = theHit;
}


// ###########################################################################################

void CBrowserWind::SetPopupPassageListToNumber(short number)
{
    MenuHandle  numberList;
    Str255      thisNum, listNum;
    int         j;

    numberList = bookNumberPane->GetMacMenuH();
    NumToString(number, thisNum);
//  itemCount = ::CountMItems(numberList);
    j = 0;
    do {
        if (j > 255) break;         // ***********************REMOVE***************************
        ::GetItem(numberList, ++j, listNum);
    } while (RelString(thisNum, listNum, FALSE, FALSE) != 0);   
    popupWasClickedFlag = FALSE;
    bookNumberPane->SetValue(j);    // sends a popup message but we don't want action
    popupWasClickedFlag = TRUE;
    bookNumberPane->Draw(NULL);
}

// ###########################################################################################
//==================================================================================

void CBrowserWind::DisplayTitles(short volNum)
{
    char        *bookName;
    Str255      tempString;
    MenuHandle  numberList;
    hit         scanHit;
    short       itemCount;
    int         j;

    bookName = mainApp->textFetcher->GetBookName(volNum);
    CToPascal(tempString, bookName);
    bookTitlePane->SetDescriptor(tempString);
    bookTitlePane->Draw(NULL);
    
    numberList = bookNumberPane->GetMacMenuH();
//  if (numberList)
//      DisposeMenu(numberList);
//  numberList = NewMenu(1, "\p");
    itemCount = ::CountMItems(numberList);

    for (j = 0; j < itemCount; j++)         // empty out old menu to add NEW numbers
        ::DelMenuItem(numberList, 1);
    
    itemCount = 0;
    while (theHits->ReportNextPassageHit(scanHit)) {
        ::NumToString(scanHit.passage, tempString);
        ::AppendMenu(numberList, tempString);
        itemCount++;
        if (itemCount > 255) break;  // <--  don't add more than 1000 for now!
                                    // **************************REMOVE*************************
    }
    bookNumberPane->SetMaxValue(itemCount);
    theHits->ResetVolume(volNum);
    
    bookNumberPane->Draw(NULL);     
}

//==================================================================================

void CBrowserWind::BuildHitOffsetList(hit theHit)
{
    hit newHit; 
    hitOffsetList.Reset();
    newHit = theHit;

    hitOffsetList.AppendHit(theHit.word);
    // first move backwards to find all previous hits in passage
    while ((theHits->ReportPreviousHit(newHit)) && (newHit.SamePassageAs(theHit)))
        hitOffsetList.AppendHit(newHit.word);

    // get back where we were
    while (!(newHit == theHit))
        theHits->ReportNextHit(newHit);
    
    // move forwards to find all next hits in passage
    while ((theHits->ReportNextHit(newHit)) && (newHit.SamePassageAs(theHit)))
        hitOffsetList.AppendHit(newHit.word);

    // get back where we were
    while (!(newHit == theHit))
        theHits->ReportPreviousHit(newHit);

}
//==================================================================================

