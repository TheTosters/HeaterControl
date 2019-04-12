#pragma once
#include "display.h"
#include "observable.h"
#include <functional>

enum class SelectedScreen {
  DEFAULT, STATUS
};

//used to observer request to render and update display
//id - tells which screen send request to display.update
//redraw - should render be called before display.Update ?
using ScreenRedraw = Observable<SelectedScreen, bool>;

//used to observer request to change screen:
// id - tells to which screen navigate
using ScreenSelect = Observable<SelectedScreen>;

class Screen : public ScreenRedraw,
               public ScreenSelect {
public:
  const SelectedScreen id;

  Screen(Display& display, const SelectedScreen id) :
    id(id), display(display) {
  }

protected:
  Display& display;
};
