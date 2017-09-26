#include <stdio.h>
#include <math.h>
#include "oniguruma.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <string>

using namespace emscripten;

int main(int argc, char ** argv) {
    printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
  printf("MyFunction Called\n");
}

int EMSCRIPTEN_KEEPALIVE int_sqrt(int x) {
    return sqrt(x);
}

void EMSCRIPTEN_KEEPALIVE search(const char* data, size_t position, size_t end) {
    regex_t* regex_;
    std::string source = "e";
    OnigErrorInfo error;
    const UChar* sourceData = (const UChar*)source.data();
    int status = onig_new(&regex_, sourceData, sourceData + source.length(),
                          ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
                          ONIG_SYNTAX_DEFAULT, &error);
    const UChar* searchData = reinterpret_cast<const UChar*>(data);
    OnigRegion* region = onig_region_new();
    status = onig_search(regex_, searchData, searchData + end,
                             searchData + position, searchData + end, region,
                             ONIG_OPTION_NONE);
    printf("Status value = %d\n", status);
}

class OnigResult {
public:
    OnigResult(OnigRegion* region, int indexInScanner)
    : region_(region), indexInScanner(indexInScanner) {}

    int Count() {
      return region_->num_regs;
    }

    int LocationAt(int index) {
        int bytes = *(region_->beg + index);
        if (bytes > 0)
          return bytes;
        else
          return 0;
    }

    int LengthAt(int index) {
        int bytes = *(region_->end + index) - *(region_->beg + index);
        if (bytes > 0)
          return bytes;
        else
          return 0;
    }

    int getIndex() const { return indexInScanner; }
    void setIndex(int newIndex) { indexInScanner = newIndex; }
private:
    OnigRegion *region_;
    int indexInScanner;
};

class OnigRegExp {
public:
  OnigRegExp(int x, std::string source)
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

  void incrementX() {
    ++x;
  }

  int getX() const { return x; }
  void setX(int x_) { x = x_; }

  OnigResult* search(std::string data, size_t position, size_t end) {
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

  static std::string getStringFromInstance(const OnigRegExp& instance) {
    return instance.source;
  }

private:
  int x;
  std::string source;
  regex_t* regex_;
};

// Binding code
EMSCRIPTEN_BINDINGS(OnigRegExp) {
    class_<OnigRegExp>("OnigRegExp")
      .constructor<int, std::string>()
      .function("incrementX", &OnigRegExp::incrementX)
      .function("search", &OnigRegExp::search, allow_raw_pointers())
      .property("x", &OnigRegExp::getX, &OnigRegExp::setX)
      .class_function("getStringFromInstance", &OnigRegExp::getStringFromInstance)
      ;
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


#ifdef __cplusplus
}
#endif
