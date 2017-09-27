#ifndef SRC_ONIG_SCANNER_H_
#define SRC_ONIG_SCANNER_H_

#include "onig-result.h"
#include "onig-reg-exp.h"
#include "onig-searcher.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

class OnigScanner {
public:
  OnigScanner(std::vector<std::string> sources);

private:

  void FindNextMatch(std::string v8String, size_t v8StartLocation);
//   Local<Value> FindNextMatchSync(OnigString* onigString, Local<Number> v8StartLocation);
//   Local<Value> FindNextMatchSync(Local<String> v8String, Local<Number> v8StartLocation);
//   static Local<Value> CaptureIndicesForMatch(OnigResult* result, OnigString* source);

  std::vector<OnigRegExp*> regExps;
  OnigSearcher searcher;
};

#endif  // SRC_ONIG_SCANNER_H_
