#pragma once
#include <inttypes.h>
#include <cstdlib>
#include <initializer_list>

class IconBridge {
public:
  int width;
  int height;
  const uint8_t* data;

  IconBridge(int w, int h, const uint8_t* data)
  : width(w), height(h), data(data) {

  }
};
