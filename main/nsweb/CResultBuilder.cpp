
#include <jsoncpp/json/json.h>

#include "CResultBuilder.h"
#include "CTextExploder.h"
#include "CHitOffsetList.h"

const int OUTPUT_VERSION = 1;

std::string CResultBuilder::Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList)
{
    Json::Value results;
    results["version"] = OUTPUT_VERSION;
    results["count"] = (int)hitList->ReportTotalHits();
    results["errors"] = Json::Value(Json::arrayValue);

    long *hitsPerVol = hitList->ReportHitsPerVol();

    Json::Value volumes = Json::Value(Json::arrayValue);

    CTextExploder textExploder;
    CHitOffsetList hitOffsetList;
    textExploder.Setup(textFetch, giantTable, &hitOffsetList);

    int j = 0;

    for (int i = 0; i < MAX_VOLUMES; ++i) {
        Json::Value volume;
        volume["name"] = textFetch->GetBookName(i);
        volume["count"] = (int)hitsPerVol[i]; 

        Json::Value hits = Json::Value(Json::arrayValue);
        if (0 && hitsPerVol[i] > 0) {
            hitList->ResetVolume(i);
            hit h;
            while (hitList->ReportNextHit(h)) {
                Json::Value word = Json::Value(Json::arrayValue);
                word.append((int)h.passage);
                word.append((int)h.word);
                hits.append(word);
            }
        }
        volume["hits"] = hits;
        volumes.append(volume);
    }

    results["volumes"] = volumes;

            textExploder.PrintPassageNumber(testhit);


    Json::FastWriter writer;
    return writer.write(results);
}
