#include "onig-reg-exp.h"

using namespace emscripten;

OnigRegExp::OnigRegExp(std::string source)
    : source(source)
    , regex_(NULL)
{
    OnigErrorInfo error;
    const UChar* sourceData = (const UChar*)source.data();
    int status = onig_new(&regex_, sourceData, sourceData + source.length(),
                          ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
                          ONIG_SYNTAX_DEFAULT, &error);
}

OnigResult* OnigRegExp::search(OnigString* str, size_t position) {
    const UChar* searchData =  reinterpret_cast<const UChar*>(str->utf8_value());
    size_t end = str->utf8_length();
    OnigRegion* region = onig_region_new();
    int status = onig_search(regex_, searchData, searchData + end,
                             searchData + position, searchData + end, region,
                             ONIG_OPTION_NONE);
    if (status != ONIG_MISMATCH) {
        return new OnigResult(region, -1);
    } else {
        // printf("Status value = %d\n", status);
        onig_region_free(region, 1);
    }
}

// Binding code
EMSCRIPTEN_BINDINGS(OnigRegExp) {
    class_<OnigRegExp>("OnigRegExp")
      .constructor<std::string>()
      .function("search", &OnigRegExp::search, allow_raw_pointers())
      ;
}
