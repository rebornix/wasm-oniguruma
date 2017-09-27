#include "oniguruma.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include "onig-result.h"

using namespace emscripten;

OnigResult::OnigResult(OnigRegion* region, int indexInScanner)
    : region_(region), indexInScanner(indexInScanner) {}

int OnigResult::Count() {
    return region_->num_regs;
}

int OnigResult::LocationAt(int index) {
    int bytes = *(region_->beg + index);
    if (bytes > 0)
        return bytes;
    else
        return 0;
}

int OnigResult::LengthAt(int index) {
    int bytes = *(region_->end + index) - *(region_->beg + index);
    if (bytes > 0)
        return bytes;
    else
        return 0;
}

EMSCRIPTEN_BINDINGS(OnigResult) {
    class_<OnigResult>("OnigResult")
      .constructor<OnigRegion*, int>(allow_raw_pointers())
      .function("LengthAt", &OnigResult::LengthAt)
      .function("LocationAt", &OnigResult::LocationAt)
      .function("Count", &OnigResult::Count)
      .property("index", &OnigResult::getIndex, &OnigResult::setIndex)
      ;
}