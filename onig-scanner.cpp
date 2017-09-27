#include "onig-scanner.h"

OnigScanner::OnigScanner(std::vector<std::string> sources) {
    int length = sources->size();
    regExps.resize(length);

    std::vector< std::string >::iterator iter = sources.begin();
    int index = 0;
    while (iter < sources.end()) {
        regExps[i] = new OnigRegExp(*iter);
        ++index;
        ++iter;
    }

    searcher = new OnigSearcher(regExps);
}

EMSCRIPTEN_BINDINGS(OnigScanner) {
    class_<OnigScanner>("OnigScanner")
      .constructor<std::vector<std::string>>(allow_raw_pointers())
      ;
}