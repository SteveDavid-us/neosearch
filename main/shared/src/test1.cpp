#include <iostream>
#include "CGiant.h"
#include "NeoException.h"
#include "CTextFetch.h"
#include "CTextExploder.h"
#include "CHitList.h"
#include "CHitOffsetList.h"

using namespace std;

int main (int argc, char *argv)
{
    CGiant *giantTable;
    CTextFetch *textFetcher;

    try {
        cout << "Alloc CGiant" << endl;
        giantTable = new CGiant();
        cout << "Alloc CTextFetch" << endl;
        textFetcher = new CTextFetch();
    } catch (CException f) {
        cout << f.Diagnostic() << endl;;
        cout << f.WhatFailed() << endl; 
        return 1;
    }

    CTextExploder textExploder;
    CHitOffsetList hitOffsetList;
    cout << "Setup textExploder" << endl;
    textExploder.Setup(textFetcher, giantTable, &hitOffsetList);
    hit testhit;
    testhit.Set(9, 1, 1);
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
    return 0;
}
