#pragma once

#include <functional>
#include <vector>

template<typename Observer>
class Observable {
public:
  template <typename... Args>
  void notify(Args&&... args) {
    for(Observer& obs : observers) {
      obs( std::forward<Args>(args)... );
    }
  }

  void addObserver(Observer obs) {
    observers.push_back(obs);
  }

private:
  std::vector<Observer> observers;
};
