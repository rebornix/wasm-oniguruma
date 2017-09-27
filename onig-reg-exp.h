#ifndef SRC_ONIG_REG_EXP_H_
#define SRC_ONIG_REG_EXP_H_

#include "oniguruma.h"
#include "onig-result.h"
#include <string>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <stdio.h>

class OnigRegExp {
public:
  OnigRegExp(int x, std::string source);
  void incrementX();

  int getX() const;
  void setX(int x_);

  OnigResult* search(std::string data, size_t position, size_t end);

private:
  int x;
  std::string source;
  regex_t* regex_;
};

#endif  // SRC_ONIG_REG_EXP_H_
