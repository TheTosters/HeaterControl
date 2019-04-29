#pragma once

#include <assert.h>

//TODO: rethink existence of this
//This indented to be used in debug only, probably need to reconsider this later
class SingleInstance {
#ifndef NDEBUG
public:
  SingleInstance() {
    //Only one instance at runtime is allowed. This is effectively singleton
    //but for now I don't have idea how to do it better without introducing
    //getInstance
    assert(instance == nullptr);
    instance = this;
  }

private:
  SingleInstance* instance = nullptr;
#endif
};
