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
        int hitStart = hitOffsetList.GetStartOffsetIdx(i); 
        // Find and add context before the hit
        int preStart = std::max<int>(hitStart - preContext, lastOutput);
        preStart = std::max<int>(preStart, 0);
        for (;preStart > 0; --preStart) {
            if (isspace(passage[preStart])) {
                break;
            }
        }
        OutputRange(output, passage, preStart, hitStart);
        
        // Add actual hit text in bold
        int hitEnd = hitOffsetList.GetEndOffsetIdx(i); 
        output << "<mark>";
        OutputRange(output, passage, hitStart, hitEnd);
        output << "</mark>";
        
        // Add context following this hit (avoiding subsequent hits)
        int postEnd = std::min<int>(hitEnd + postContext, length);
        for (;postEnd < length; ++postEnd) {
            if (isspace(passage[postEnd])) {
                break;
            }
        }
        int nextHitStart = length;
        if (i + 1 < hits) {
            nextHitStart = hitOffsetList.GetStartOffsetIdx(i + 1);
            postEnd = std::min(postEnd, nextHitStart);
        }
        OutputRange(output, passage, hitEnd, postEnd);
        lastOutput = postEnd;

        if (lastOutput != nextHitStart) {
            output << " &hellip;";
        }
    }

    return output.str();
}

std::string CResultBuilder::Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList)
{
    Json::Value results;
    results["version"] = OUTPUT_VERSION;
    results["count"] = (int)hitList->ReportTotalPassagesHit();
    results["errors"] = Json::Value(Json::arrayValue);
    results["total_hits"] = (int)hitList->ReportTotalHits();

    long *hitsPerVol = hitList->ReportHitsPerVol();
    long passageHitsPerVol[MAX_VOLUMES] = {0};
    hitList->ReportPassagesHitPerVol(passageHitsPerVol);

    CTextExploder textExploder;
    CHitOffsetList hitOffsetList;
    textExploder.Setup(textFetch, giantTable, &hitOffsetList);

    // Complete hits by volume
    Json::Value volumes = Json::Value(Json::arrayValue);
    for (int vol = 0; vol < MAX_VOLUMES; ++vol) {
        Json::Value volume;
        volume["name"] = textFetch->GetBookName(vol);
        volume["count"] = (int)hitsPerVol[vol]; 
        volumes.append(volume);
    }
    results["volumes"] = volumes;

    // Requested results
    Json::Value passages = Json::Value(Json::arrayValue);
    unsigned int passageIndex = 0;
    unsigned int lastPassage = firstPassage + passageCount;

    for (int vol = 0; vol < MAX_VOLUMES && passageIndex < lastPassage; ++vol) {
        if (passageIndex + passageHitsPerVol[vol] < firstPassage) {
            passageIndex += passageHitsPerVol[vol];
            continue;
        }

        hitList->ResetVolume(vol);
        hit volHit;
        bool hitActive = false;
        int activePassage = -1;

        for (int volPas = 0; volPas < passageHitsPerVol[vol] &&
                passageIndex < lastPassage; ++volPas, ++passageIndex) {

            int nextPassage = -1;

            if (hitActive) {
                if (activePassage < 0) {
                    throw CException("No active passage");
                }
            } else {
                if (!hitList->ReportNextHit(volHit)) {
                    throw CException("Missing hit");
                }
                hitActive = true;
                activePassage = volHit.passage;
            }

            if (passageIndex >= firstPassage) {
                hitOffsetList.Reset();
                hitOffsetList.AppendHit(volHit.word);
            }

            hit nextHit;
            while (hitList->ReportNextHit(nextHit)) {
                if (nextHit.passage != activePassage) {
                    nextPassage = nextHit.passage;
                    break;
                }
                if (passageIndex >= firstPassage) {
                    hitOffsetList.AppendHit(nextHit.word);
                }
            }

            if (passageIndex >= firstPassage) {
                cstr passageStr = textExploder.RetrievePassageNumber(volHit);

                Json::Value passage = Json::Value();
                passage["volume"] = textFetch->GetBookName(vol);
                passage["passage"] = activePassage;
                passage["count"] = hitOffsetList.GetListSize();
                passage["text"] = WriteHit(passageStr.text, passageStr.length, hitOffsetList);
                passages.append(passage);
            }

            hitActive = (nextPassage >= 0);
            if (hitActive) {
                volHit = nextHit;
                activePassage = nextPassage;
            }
        }
    }
   
    results["passages"] = passages;

    Json::FastWriter writer;
    std::string output = writer.write(results);

    return output;
}
