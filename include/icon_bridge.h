#pragma once
#include <inttypes.h>
#include <cstdlib>
#include <initializer_list>

template<typename T> class GenIconBridge {
public:
  int width;
  int height;
  const T* data;

  GenIconBridge(int w, int h, std::initializer_list<T> il);
};

template<typename T>
GenIconBridge<T>::GenIconBridge(int w, int h, std::initializer_list<T> li)
: width(w), height(h), data(li.begin())
{

}

using IconBridge = GenIconBridge<uint8_t>;
