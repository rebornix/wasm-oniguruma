#ifndef SRC_ONIG_SCANNER_H_
#define SRC_ONIG_SCANNER_H_

#include "onig-result.h"
#include "onig-reg-exp.h"
#include "onig-searcher.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>


class CaptureIndice {
public:
    CaptureIndice(int index, int start, int end, int length)
    : index(index) , start(start) , end(end) , length(length) {}
    int getIndex() const { return index; }
    void setIndex(int newIndex) { index = newIndex; }
    int getStart() const { return start; }
    void setStart(int newStart) { start = newStart; }
    int getEnd() const { return end; }
    void setEnd(int newEnd) { end = newEnd; }
    int getLength() const { return length; }
    void setLength(int newLength) { length = newLength; }

private:
    int index;
    int start;
    int end;
    int length;
};

class CaptureResult {
public:
    CaptureResult(OnigResult* result) {
        index = result->getIndex();
        int resultCount = result->Count();
        std::vector<CaptureIndice*> arr;

        for(int i = 0; i < resultCount; ++i) {
            int captureStart = result->LocationAt(i);
            int captureEnd = result->LocationAt(i) + result->LengthAt(i);

            arr.push_back(new CaptureIndice(i, captureStart, captureEnd, captureEnd - captureStart));
        }

        captureIndices = arr;
    }
    int getIndex() const { return index; }
    void setIndex(int newIndex) { index = newIndex; }

    int Count() {
        return captureIndices.size();
    }

    CaptureIndice* IndiceAt(int index) {
        return captureIndices[index];
    }

    // std::vector<CaptureIndice*> getCaptureIndices() const { return captureIndices; }
    // void setCaptureIndices(std::vector<CaptureIndice*> newCaptureIndices) { captureIndices = newCaptureIndices; }
private:
    int index;
    std::vector<CaptureIndice*> captureIndices;
};

class OnigScanner {
public:
  OnigScanner(std::vector<std::string> sources);

  CaptureResult* FindNextMatchSync(std::string v8String, size_t v8StartLocation);
private:

//   void FindNextMatch(std::string v8String, size_t v8StartLocation);
//   Local<Value> FindNextMatchSync(Local<String> v8String, Local<Number> v8StartLocation);
//   static Local<Value> CaptureIndicesForMatch(OnigResult* result, OnigString* source);

  std::vector<OnigRegExp*> regExps;
  OnigSearcher* searcher;
};

#endif  // SRC_ONIG_SCANNER_H_
