#include "onig-reg-exp.h"

using namespace emscripten;

OnigRegExp::OnigRegExp(int x, std::string source)
    : x(x)
    , source(source)
    , regex_(NULL)
{
    OnigErrorInfo error;
    const UChar* sourceData = (const UChar*)source.data();
    int status = onig_new(&regex_, sourceData, sourceData + source.length(),
                          ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
                          ONIG_SYNTAX_DEFAULT, &error);
}

void OnigRegExp::incrementX() {
    ++x;
}

int OnigRegExp::getX() const { return x; }
void OnigRegExp::setX(int x_) { x = x_; }

OnigResult* OnigRegExp::search(std::string data, size_t position, size_t end) {
    const UChar* searchData = (const UChar*)data.data();
    OnigRegion* region = onig_region_new();
    int status = onig_search(regex_, searchData, searchData + end,
                             searchData + position, searchData + end, region,
                             ONIG_OPTION_NONE);
    if (status != ONIG_MISMATCH) {
        return new OnigResult(region, -1);
    } else {
        printf("Status value = %d\n", status);
        onig_region_free(region, 1);
    }
}

// Binding code
EMSCRIPTEN_BINDINGS(OnigRegExp) {
    class_<OnigRegExp>("OnigRegExp")
      .constructor<int, std::string>()
      .function("incrementX", &OnigRegExp::incrementX)
      .function("search", &OnigRegExp::search, allow_raw_pointers())
      .property("x", &OnigRegExp::getX, &OnigRegExp::setX)
      ;
}
