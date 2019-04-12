#pragma once

#include <functional>
#include <vector>

template<typename ... TempArg>
class Observable {
public:
  using ObsFun = std::function<void(TempArg...)>;

//  void notify(const TempArg&... args) {
//    for(ObsFun& obs : observers) {
//      obs( args... );
//    }
//  }

  void notify(TempArg... args) {
    for(ObsFun& obs : observers) {
      obs( args... );
    }
  }

  void addObserver( ObsFun&& obs) {
    observers.push_back( std::forward<ObsFun>(obs));
  }

private:
  std::vector<ObsFun> observers;
};
