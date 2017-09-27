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

    printf("Sources length = %d\n", length);
}

OnigResult* OnigScanner::FindNextMatchSync(std::string v8String, size_t v8StartLocation) {
    OnigResult* bestResult = searcher->Search(v8String, v8StartLocation);
    return bestResult;
}

EMSCRIPTEN_BINDINGS(OnigScanner) {
    register_vector<std::string>("VectorString");
    class_<OnigScanner>("OnigScanner")
      .constructor<std::vector<std::string>>(allow_raw_pointers())
      .function("findNextMatchSync", &OnigScanner::FindNextMatchSync, allow_raw_pointers())
      ;
}