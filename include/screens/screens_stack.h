#pragma once

#include "screens/screen.h"
#include "screens/default_screen.h"
#include "screens/status_screen.h"
#include "screens/time_setup_screen.h"
#include "screens/remote_config_screen.h"
#include "buttons.h"
#include "events_dispatcher.h"
#include <variant>
#include <forward_list>
#include <algorithm>
#include <type_traits>

using AnyScreen = std::variant<
    DefaultScreen, StatusScreen, TimeSetupScreen, RemoteConfigScreen>;

class ScreensStack {
public:
  ScreensStack(Display& display) :
    display(display) {
  }

  void render() {
    ensureMainThread();
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
    static_assert(std::is_rvalue_reference<decltype(screen)>::value,
        "Arument need to be rvalue ref");
    T& result = std::get<T>(screens.emplace_front(std::forward<T>(screen)));
    attachRefreshLogic(result);
    attachSwapScreenLogic(result);
    return result;
  }

  template<typename T>
  void attachSwapScreenLogic(T& screen) {
    screen.ScreenSelect::addObserver(
      [this](const SelectedScreen id) {
        selectScreen(id);
        if (display.isPowered()){
          render();
          display.update();
        }
    });
  }

  template<typename T>
  void attachRefreshLogic(T& screen) {
    screen.ScreenRedraw::addObserver(
      [this](const SelectedScreen id, const bool redraw) {
      if (display.isPowered() and currentScreenId() == id){
        if (redraw) {
          render();
        }
        display.update();
      }
    });
  }

  void onButtonEvent(ButtonId event) {
    if (not display.isPowered()) {
      display.sustainOn();
      render();

    } else {
      //forward event to current
      std::visit([=](auto&& screen) {screen.onButtonEvent(event);}, *currentScr);
      display.sustainOn();
      render();
    }
  }

private:
  using ScreensVector = std::forward_list<AnyScreen>;
  ScreensVector::iterator currentScr;
  ScreensVector screens;
  Display& display;
};
