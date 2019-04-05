#pragma once

#include "screens/screen.h"
#include "screens/default_screen.h"
#include <variant>
#include <forward_list>
#include <algorithm>

using AnyScreen = std::variant<DefaultScreen>;

class ScreensStack {
public:
  void render() {
    std::visit([](auto&& screen) {screen.render();}, *currentScr);
  }

  void selectScreen(SelectedScreen scrId) {
    currentScr = std::find_if(screens.begin(), screens.end(),
        [=](const auto& anyScr) {
          return std::visit([=](const auto& screen) {
            return screen.id == scrId;
          }, anyScr);
    });
  }

  template<typename T>
  T& add(T&& screen) {
    return std::get<T>(screens.emplace_front(std::forward<T>(screen)));
  }

private:
  using ScreensVector = std::forward_list<AnyScreen>;
  ScreensVector::iterator currentScr;
  ScreensVector screens;
};
