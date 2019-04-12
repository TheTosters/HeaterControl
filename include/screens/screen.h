#pragma once
#include "display.h"
#include "observable.h"
#include <functional>

enum class SelectedScreen {
  DEFAULT
};

using ScreenRedrawObserver = std::function<void(const SelectedScreen,
                                                const bool redraw)>;

class Screen : public Observable<ScreenRedrawObserver>{
public:
  const SelectedScreen id;

  Screen(Display& display, const SelectedScreen id) :
    id(id), display(display) {
  }

protected:
  Display& display;
};
