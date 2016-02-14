#include <jsoncpp/json/json.h>

#include "CResultBuilder.h"
#include "CTextExploder.h"
#include "CHitOffsetList.h"

#include "NeoException.h"

#include <iostream>
#include <sstream>

const int OUTPUT_VERSION = 1;

void OutputRange(stringstream &output, char* text, int start, int end)
{
    if (start == end) {
        return;
    }
    char backup = text[end];
    text[end] = '\0';
    output << (text + start);
    text[end] = backup;
}

std::string CResultBuilder::WriteHit(char *passage, int length, CHitOffsetList &hitOffsetList)
{
    stringstream output;

    int lastOutput = 0;
    int hits = hitOffsetList.GetListSize();

    for (int i = 0; i < hits; ++i) {
        int hitStart = hitOffsetList.GetStartOffset(i); 
        int hitEnd = hitOffsetList.GetEndOffset(i); 
        // Find and add context before the hit
        int preStart = std::max<int>(hitStart - preContext, lastOutput);
        preStart = std::max<int>(preStart, 0);
        for (;preStart >= 0; --preStart) {
            if (isspace(passage[preStart]) || ispunct(passage[preStart])) {
                break;
            }
        }
        OutputRange(output, passage, preStart, hitStart);
        
        // Add hit
        output << "<em>";
        OutputRange(output, passage, hitStart, hitEnd);
        output << "</em>";
        
        // Add context following this hit (avoiding subsequent hits)
        int postEnd = std::min<int>(hitEnd + postContext, length);
        if (i + 1 < hits) {
            int nextHitStart = hitOffsetList.GetStartOffset(i + 1);
            postEnd = std::min(postEnd, nextHitStart);
        }
        OutputRange(output, passage, hitEnd, postEnd);
        lastOutput = postEnd;

        if (lastOutput != length) {
            output << "&hellip;";
        }
    }

    return output.str();
}

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

    // Complete hits by volume
    for (int vol = 0; vol < MAX_VOLUMES; ++vol) {
        Json::Value volume;
        volume["name"] = textFetch->GetBookName(vol);
        volume["count"] = (int)hitsPerVol[vol]; 
        volumes.append(volume);
    }
    results["volumes"] = volumes;

    // Requested results
    Json::Value hits = Json::Value(Json::arrayValue);
    int hitIndex = 0;
    int lastHit = firstHit + hitCount;

    for (int vol = 0; vol < MAX_VOLUMES && hitIndex < lastHit; ++vol) {
        int lastPassage = -1;
        cstr passage;
        hitList->ResetVolume(vol);
        for (int volHit = 0; volHit < hitsPerVol[vol] && hitIndex < lastHit; ++volHit) {

            hit theHit;
            if (!hitList->ReportNextHit(theHit)) {
                throw CException("Missing hit");
            }

            if (theHit.passage == lastPassage) {
                ++hitIndex;
                continue;
            }

            hitOffsetList.Reset();
            passage = textExploder.RetrievePassageNumber(theHit);
            lastPassage = theHit.passage;

            Json::Value result = Json::Value();
            result["volume"] = textFetch->GetBookName(vol);
            result["passage"] = theHit.passage;
            result["count"] = hitOffsetList.GetListSize();
            result["text"] = WriteHit(passage.text, passage.length, hitOffsetList);
            hits.append(result);
            ++hitIndex;
        }
    }
   
    results["hits"] = hits;

    Json::FastWriter writer;
    std::string output = writer.write(results);

    return output;
}
