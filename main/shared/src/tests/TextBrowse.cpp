#include <iostream>
#include "CGiant.h"
#include "NeoException.h"
#include "CTextFetch.h"
#include "CTextExploder.h"
#include "CHitList.h"
#include "CHitOffsetList.h"
#include "CTarget.h"

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
    theTarget.SetString("amo");

    theHits = giantTable->FindHitsWithSubstringOf(theTarget);

    if (!theHits) {
        cout << "got not'in" << endl;
        return 1;    
    }

    hit testhit;

    cout << "total hits: " << theHits->ReportTotalHits() << endl;

    long *hitsPerVol = theHits->ReportHitsPerVol();

    for (int i = 0; i < MAX_VOLUMES; ++i) {
        if (hitsPerVol[i] > 0) {
            printf("Book %d, %s, %ld hits.\n", i, textFetcher->GetBookName(i), hitsPerVol[i]);
            theHits->ResetVolume(i);
            while (theHits->ReportNextHit(testhit)) {
                cout << "vol = " << testhit.volume << " pas = " << testhit.passage << " word = " << testhit.word << endl;
            }
        }
    }
    if (1)
    {
        CTextExploder textExploder;
        CHitOffsetList hitOffsetList;
        cout << "Setup textExploder" << endl;
        textExploder.Setup(textFetcher, giantTable, &hitOffsetList);
#ifndef DEBUG
        try
#endif
        {
            cout << "PrintPassageNumber" << endl;
            textExploder.PrintPassageNumber(testhit);
        }
#ifndef DEBUG
        catch (CException f) {
            cout << f.Diagnostic() << endl;
            cout << f.WhatFailed() << endl;
        }
#endif
    }

    return 0;
}

