#ifndef SRC_ONIG_SEARCHER_H_
#define SRC_ONIG_SEARCHER_H_

#include "onig-reg-exp.h"
#include "onig-result.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

class OnigSearcher {
public:
  OnigSearcher(std::vector<OnigRegExp*> regExps)
    : regExps(regExps) {}


  OnigResult* Search(std::string source, int charOffset);

 private:
  std::vector<OnigRegExp*> regExps;
};

#endif  // SRC_ONIG_SEARCHER_H_
