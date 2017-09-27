#ifndef SRC_ONIG_RESULT_H_
#define SRC_ONIG_RESULT_H_

#include "oniguruma.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

class OnigResult {
public:
    OnigResult(OnigRegion* region, int indexInScanner);
    int Count();

    int LocationAt(int index);
    int LengthAt(int index);

    int getIndex() const { return indexInScanner; }
    void setIndex(int newIndex) { indexInScanner = newIndex; }
private:
    OnigRegion *region_;
    int indexInScanner;
};

#endif  // SRC_ONIG_RESULT_H_
