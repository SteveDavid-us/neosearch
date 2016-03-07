#include <jsoncpp/json/json.h>

#include "CResultBuilder.h"
#include "CTextExploder.h"
#include "CHitOffsetList.h"

#include "NeoException.h"

#include <iostream>
#include <sstream>

const int OUTPUT_VERSION = 3;

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

std::string CResultBuilder::WritePassage(char *passage, int length, CHitOffsetList &hitOffsetList)
{
    stringstream output;

    int lastOutput = 0;
    int hits = hitOffsetList.GetListSize();

    for (int i = 0; i < hits; ++i) {
        int hitStart = hitOffsetList.GetStartOffsetIdx(i); 
        OutputRange(output, passage, lastOutput, hitStart);
        
        // Add actual hit text in bold
        int hitEnd = hitOffsetList.GetEndOffsetIdx(i); 
        output << "<mark>";
        OutputRange(output, passage, hitStart, hitEnd);
        output << "</mark>";
        lastOutput = hitEnd;
    }
    OutputRange(output, passage, lastOutput, length);

    stringstream formatted;
    char c;
    while (output.get(c)) {
        switch (c) {
        case '\n':
            formatted << "<br>";
            break;
        default:
            formatted << c;
            break; 
        }
    }

    return formatted.str();
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

void CResultBuilder::LoadResults(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList, Json::Value &passages)
{
    CTextExploder textExploder;
    CHitOffsetList hitOffsetList;
    textExploder.Setup(textFetch, giantTable, &hitOffsetList);

    long passageHitsPerVol[MAX_VOLUMES] = {0};
    hitList->ReportPassagesHitPerVol(passageHitsPerVol);

    unsigned int passageIndex = 0;
    unsigned int lastPassage = firstPassage + passageCount;

    for (unsigned int i = 0; i < MAX_VOLUMES && passageIndex < lastPassage; ++i) {
        int vol = CTextFetch::BookOrder[i];
        if (!(volumeFilter.empty() || (volumeFilter.find(vol) != volumeFilter.end()))) {
            continue;
        }
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
                passage["book"] = textFetch->GetBookName(vol);
                passage["volume"] = vol;
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
}

void CResultBuilder::LoadPassage(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList, Json::Value &passages)
{
    CTextExploder textExploder;
    CHitOffsetList hitOffsetList;
    textExploder.Setup(textFetch, giantTable, &hitOffsetList);

    hit passageHit;
    hitOffsetList.Reset();
    if (hitList->SeekPassage((short)volume, (short)passage, passageHit)) {
        do {
            hitOffsetList.AppendHit(passageHit.word);
        } while (hitList->ReportNextHit(passageHit) && passageHit.passage == passage);
        passageHit.volume = (short)volume;
        passageHit.passage = (short)passage;
    } else {
        passageHit.volume = (short)volume;
        int highest = textFetch->GetHighestPassageNumber((short)volume);
        passageHit.passage = passage > highest ? highest : passage;
        if (passage == 0) {
            long len = 0;
            int* result = textFetch->ReadPassage((short)volume, (short)passage, len);
            if (len == 0) {
                passageHit.passage = 1;
            } 
        }
    }

    Json::Value result = Json::Value();
    cstr passageStr = textExploder.RetrievePassageNumber(passageHit);
    result["book"] = textFetch->GetBookName((short)volume);
    result["volume"] = (int)passageHit.volume;
    result["passage"] = (int)passageHit.passage;
    result["count"] = hitOffsetList.GetListSize();
    result["text"] = WritePassage(passageStr.text, passageStr.length, hitOffsetList);

    passages.append(result);
}

std::string CResultBuilder::Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList)
{
    Json::Value results;
    results["version"] = OUTPUT_VERSION;

    long *hitsPerVol = hitList->ReportHitsPerVol();
    long passageHitsPerVol[MAX_VOLUMES] = {0};
    hitList->ReportPassagesHitPerVol(passageHitsPerVol);

    if (volumeFilter.empty()) {
        results["count"] = (unsigned int)hitList->ReportTotalPassagesHit();
    } else {
        long filteredPassages = 0;
        for (unsigned int vol = 0; vol < MAX_VOLUMES; ++vol) {
            if (volumeFilter.find(vol) != volumeFilter.end()) {
                filteredPassages += passageHitsPerVol[vol];
            }
        }
        results["count"] = (unsigned int)filteredPassages;
    }
    results["errors"] = Json::Value(Json::arrayValue);
    results["total_hits"] = (int)hitList->ReportTotalHits();

    // Complete hits by volume, active (checked) first
    Json::Value volumes = Json::Value(Json::arrayValue);
    for (int active = 1; active >=0; --active) {
        for (int i = 0; i < MAX_VOLUMES; ++i) {
            int vol = CTextFetch::BookOrder[i];
            if (active != (volumeFilter.find(vol) != volumeFilter.end())) {
                continue;
            }
            Json::Value volume;
            volume["active"] = (bool)active;
            volume["id"] = vol;
            volume["name"] = textFetch->GetBookName(vol);
            volume["hits"] = (int)hitsPerVol[vol]; 
            volumes.append(volume);
        }
    }
    results["volumes"] = volumes;

    Json::Value passages = Json::Value(Json::arrayValue);
    if (passage >= 0 && volume >= 0) {
        results["type"] = "passage";
        LoadPassage(giantTable, textFetch, hitList, passages);
    } else {
        results["type"] = "results";
        LoadResults(giantTable, textFetch, hitList, passages);
    }
    results["passages"] = passages;

    Json::FastWriter writer;
    std::string output = writer.write(results);

    return output;
}
