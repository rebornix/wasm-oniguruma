#include "onig-searcher.h"

using namespace emscripten;

OnigResult* OnigSearcher::Search(std::string source, int charOffset) {
  int bestLocation = 0;
  OnigResult* bestResult;

  std::vector< OnigRegExp* >::iterator iter = regExps.begin();
  int index = 0;
  while (iter < regExps.end()) {
    OnigRegExp *regExp = (*iter);
    OnigResult* result = regExp->search(source, charOffset, source.length());
    if (result != NULL && result->Count() > 0) {
      int location = result->LocationAt(0);

      if (bestResult == NULL || location < bestLocation) {
        bestLocation = location;
        bestResult = result;
        bestResult->setIndex(index);
      }

      if (location == charOffset) {
        break;
      }
    }
    ++index;
    ++iter;
  }

  return bestResult;
}

EMSCRIPTEN_BINDINGS(OnigSearcher) {
    class_<OnigSearcher>("OnigSearcher")
      .constructor<std::vector<OnigRegExp*>>(allow_raw_pointers())
      .function("Search", &OnigSearcher::Search, allow_raw_pointers())
      ;
}