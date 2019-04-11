#pragma once

#include "screens/screen.h"
#include "screens/default_screen.h"
#include "buttons.h"
#include <variant>
#include <forward_list>
#include <algorithm>
#include "events_dispatcher.h"
using AnyScreen = std::variant<DefaultScreen>;

class ScreensStack {
public:
  ScreensStack(Display& display) :
    display(display) {
  }

  void render() {
    std::visit([](auto&& screen) {
      screen.render();
    }, *currentScr);
  }

  void selectScreen(SelectedScreen scrId) {
    currentScr = std::find_if(screens.begin(), screens.end(),
        [=](const auto& anyScr) {
          return std::visit([=](const auto& screen) {
            return screen.id == scrId;
          }, anyScr);
    });
  }

  SelectedScreen currentScreenId() {
    return std::visit([](auto&& screen) {return screen.id;}, *currentScr);
  }

  template<typename T>
  T& add(T&& screen) {
    return std::get<T>(screens.emplace_front(std::forward<T>(screen)));
  }

  void onButtonEvent(ButtonId event) {
    if (not display.isPowered()) {
      render();

    } else {
      //forward event to current
      std::visit([=](auto&& screen) {screen.onButtonEvent(event);}, *currentScr);
      render();
    }
  }
private:
  using ScreensVector = std::forward_list<AnyScreen>;
  ScreensVector::iterator currentScr;
  ScreensVector screens;
  Display& display;
};