#include "onig-scanner.h"
using namespace emscripten;

OnigScanner::OnigScanner(std::vector<std::string> sources)
    : searcher(NULL)
{
    int length = sources.size();
    regExps.resize(length);

    std::vector< std::string >::iterator iter = sources.begin();
    int index = 0;
    while (iter < sources.end()) {
        regExps[index] = new OnigRegExp(*iter);
        ++index;
        ++iter;
    }

    searcher = new OnigSearcher(regExps);
}

CaptureResult* OnigScanner::FindNextMatchSync(std::string v8String, size_t v8StartLocation) {
    OnigResult* bestResult = searcher->Search(v8String, v8StartLocation);
    if (bestResult != NULL) {
        return new CaptureResult(bestResult);
    } else {
        return NULL;
    }
}

EMSCRIPTEN_BINDINGS(CaptureIndice) {
    class_<CaptureIndice>("CaptureIndice")
      .constructor<int, int, int, int>(allow_raw_pointers())
      .property("index", &CaptureIndice::getIndex, &CaptureIndice::setIndex)
      .property("start", &CaptureIndice::getStart, &CaptureIndice::setStart)
      .property("end", &CaptureIndice::getEnd, &CaptureIndice::setEnd)
      .property("length", &CaptureIndice::getLength, &CaptureIndice::setLength)
      ;
};

EMSCRIPTEN_BINDINGS(CaptureResult) {
    class_<CaptureResult>("CaptureResult")
      .constructor<OnigResult*>(allow_raw_pointers())
      .property("index", &CaptureResult::getIndex, &CaptureResult::setIndex)
      // returning array/vector is a bummer, here we return two functions.
      .function("Count", &CaptureResult::Count)
      .function("IndiceAt", &CaptureResult::IndiceAt, allow_raw_pointers())

    //   .property("captureIndices", &CaptureResult::getCaptureIndices, &CaptureResult::setCaptureIndices)
      ;
};

EMSCRIPTEN_BINDINGS(OnigScanner) {
    register_vector<std::string>("VectorString");
    class_<OnigScanner>("OnigScanner")
      .constructor<std::vector<std::string>>(allow_raw_pointers())
      .function("_findNextMatchSync", &OnigScanner::FindNextMatchSync, allow_raw_pointers())
      ;
}