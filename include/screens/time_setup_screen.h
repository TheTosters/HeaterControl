#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "calendar.h"
#include "sensors/sensor_factory.h"
#include <string>
#include <stdint.h>
#include <sstream>
#include <ctime>
#include <chrono>

class TimeSetupScreen : public Screen {
public:
  TimeSetupScreen(Display& display, Calendar& calendar) :
    Screen(display, SelectedScreen::TIME_SETUP), calendar(calendar)
  {
  }

  void render() {
    if (not calendarSynced) {
      syncWithCalendar();
    }
    display.clear();
    display.selectFont(SelectedFont::SMALL);
    display.drawString(0, 0, "Ustawianie czasu");
    drawChoice("D:", time.dayName, LINE_1_Y, EDIT_DAY);
    printHour();
    drawChoice("Powrot", "", LINE_3_Y, PREV_SCREEN);
    ScreenRedraw::notify(id, false);
  }

  void onButtonEvent(ButtonId event) {
    if (editing) {
      if (selectedItem == EDIT_DAY) {
        onEditDay(event);

      } else {
        onEditTime(event);
      }

    } else {
      doNavigate(event);
    }
  }
private:
  using ChronoDays = std::chrono::duration<int64_t, std::ratio<86400>>;
  static constexpr int OFFSET = 4;
  static constexpr int LINE_1_Y = 12;
  static constexpr int LINE_2_Y = 24;
  static constexpr int LINE_3_Y = 50;

  DecodedTime time;

  enum EditChoice {
    EDIT_DAY, EDIT_TIME, PREV_SCREEN
  } selectedItem {EDIT_DAY};

  enum TimeIndices {
    HOUR, MINUTE, EXIT_EDIT
  } timeEditIndex {HOUR};

  bool editing{false};
  bool editingTimeElement{false};
  bool calendarSynced{false};
  Calendar& calendar;

  void syncWithCalendar() {
    calendarSynced = true;
    time = calendar.getDecodedTime();
  }

  void setNewSystemTime() {
    std::chrono::seconds result {time.hour + time.minute + time.dayOfWeek};
    calendar.setRawTime( result );
  }

  void onEditTime(ButtonId event) {
    if (editingTimeElement) {
      if (timeEditIndex == HOUR) {
        onEditHour(event);

      } else {
        onEditMinute(event);
      }

    } else {
      onNavigateTimeIndex(event);
    }
  }

  void onEditHour(ButtonId event) {
    switch(event) {
      case ButtonId::OK:
        editingTimeElement = false;
        break;

      case ButtonId::PREV:
        changeTimeComponent(time.hour, -1, 23);
        break;

      case ButtonId::NEXT:
        changeTimeComponent(time.hour, 1, 23);
        break;
    }
    ScreenRedraw::notify(id, true);
  }

  void onEditMinute(ButtonId event) {
    switch(event) {
      case ButtonId::OK:
        editingTimeElement = false;
        break;

      case ButtonId::PREV:
        changeTimeComponent(time.minute, -1, 59);
        break;

      case ButtonId::NEXT:
        changeTimeComponent(time.minute, 1, 59);
        break;
    }
    ScreenRedraw::notify(id, true);
  }

  template<typename T>
  void changeTimeComponent(T& element, int step, int max) {
    element += T{step};
    if (element.count() < 0) {
      element = T{max};
    }
    if (element.count() > max) {
      element = T{0};
    }
  }

  void onNavigateTimeIndex(ButtonId event) {
    switch(event) {
      case ButtonId::OK:
        if (timeEditIndex == EXIT_EDIT) {
          setNewSystemTime();
          editing = false;

        } else {
          editingTimeElement = true;
        }
        break;

      case ButtonId::PREV:
        prevTimeIndex();
        break;

      case ButtonId::NEXT:
        nextTimeIndex();
        break;
    }
    ScreenRedraw::notify(id, true);
  }

  void onEditDay(ButtonId event) {
    switch(event) {
      case ButtonId::OK:
        editing = false;
        setNewSystemTime();
        break;

      case ButtonId::PREV:
        changeTimeComponent(time.dayOfWeek, -1, 6);
        break;

      case ButtonId::NEXT:
        changeTimeComponent(time.dayOfWeek, 1, 6);
        break;
    }
    ScreenRedraw::notify(id, true);
  }

  void doNavigate(ButtonId event) {
    switch(event){
      case ButtonId::OK:
        if (selectedItem == PREV_SCREEN) {
          //next time when we back to this screen we should sync with calendar
          calendarSynced = false;
          ScreenSelect::notify(SelectedScreen::STATUS);

        } else {
          editing = true;
          //always even if we not editing this item, to have clean state
          timeEditIndex = HOUR;
          editingTimeElement = false;
          ScreenRedraw::notify(id, true);
        }
        break;

      case ButtonId::PREV:
        prevSelectedItem();
        ScreenRedraw::notify(id, true);
        break;

      case ButtonId::NEXT:
        nextSelectedItem();
        ScreenRedraw::notify(id, true);
        break;
    }
  }

  void prevTimeIndex() {
    if (timeEditIndex == HOUR) {
      return;
    }
    timeEditIndex = static_cast<TimeIndices>(
        static_cast<int>(timeEditIndex) - 1);
  }

  void nextTimeIndex() {
    if (timeEditIndex == EXIT_EDIT) {
      return;
    }
    timeEditIndex = static_cast<TimeIndices>(
        static_cast<int>(timeEditIndex) + 1);
  }

  void nextSelectedItem() {
    if (selectedItem == PREV_SCREEN) {
      return;
    }
    selectedItem = static_cast<EditChoice>( static_cast<int>(selectedItem) + 1);
  }

  void prevSelectedItem() {
    if (selectedItem == EDIT_DAY) {
      return;
    }
    selectedItem = static_cast<EditChoice>( static_cast<int>(selectedItem) - 1);
  }

  void printHour() {
    std::stringstream buf;

    buf << std::setfill('0') << std::setw(2) << time.hour.count() << ':'
        << std::setfill('0') << std::setw(2) << time.minute.count();

    int x = drawChoice("Czas", buf.str(), LINE_2_Y, EDIT_TIME);
    if ((not editing) or selectedItem != EDIT_TIME) {
      return;
    }
    switch(timeEditIndex) {
      case HOUR:
        display.drawString(x, LINE_2_Y, "__");
        break;

      case MINUTE:
        x += display.getStringWidth("00:");
        display.drawString(x, LINE_2_Y, "__");
        break;

      case EXIT_EDIT:
        x = display.getStringWidth("[x]");
        display.drawString(display.width() - x, LINE_2_Y, "[x]");
        break;
    }
  }

  int drawChoice(const std::string& label, const std::string& text, int y,
      EditChoice choice) {
    int labelLen = display.getStringWidth(label);
    display.drawString(0, y, label);
    std::string tail;
    if (editing and selectedItem == choice) {
      display.drawString(labelLen + OFFSET, y, "[");
      labelLen += display.getStringWidth("[");
      tail = "]";

    } else {
      tail = "<-";
    }

    display.drawString(labelLen + OFFSET, y, text);
    if (selectedItem == choice) {
      const int textLen = display.getStringWidth(text);
      display.drawString(labelLen + OFFSET + textLen, y, tail);
    }
    return labelLen + OFFSET;
  }
};

