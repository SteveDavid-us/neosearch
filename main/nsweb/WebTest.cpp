#include <iostream>
#include "CGiant.h"
#include "NeoException.h"
#include "CTextFetch.h"
#include "CTextExploder.h"
#include "CHitList.h"
#include "CHitOffsetList.h"
#include "CTarget.h"
#include "CResultBuilder.h"

using namespace std;

int main (int argc, char **argv)
{
    CGiant *giantTable;
    CTextFetch *textFetcher;
    CHitList *theHits = NULL;
    CTarget theTarget;

    int mode;

    try {
        cout << "Alloc CGiant" << endl;
        giantTable = new CGiant();
        cout << "Alloc CTextFetch" << endl;
        textFetcher = new CTextFetch();
    } catch (CException f) {
        cout << f.Diagnostic() << endl;
        cout << f.WhatFailed() << endl; 
        return 1;
    }

    giantTable->SetAmbiguityChecking(true);
    giantTable->SetDisambiguationChecking(false);

    mode = CREATE_NEW;

    bool finished = false;
    bool failure = false;

    giantTable->SetSearchMode(mode, theHits, 25);
    theTarget.SetString("amo*");

    theHits = giantTable->FindHitsWithMatchOf(theTarget);

    if (!theHits) {
        cout << "got not'in" << endl;
        return 1;    
    }

    hit testhit;

    cout << "total hits: " << theHits->ReportTotalHits() << endl;

    CResultBuilder resultBuilder;
    resultBuilder.firstPassage = 0;
    resultBuilder.passageCount = 10;
    resultBuilder.volume = 20;
    resultBuilder.passage = 0;
    //resultBuilder.volumeFilter.insert(7);

    cout << "result:" << endl << resultBuilder.Write(giantTable, textFetcher, theHits);

    return 0;
}

