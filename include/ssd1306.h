#pragma once

#include <cinttypes>
#include <vector>
#include <array>
#include <string>
#include "bridges/i2c_bridge.h"
#include "bridges/font_bridge.h"
#include "bridges/icon_bridge.h"

enum SSD1306Cmd : uint8_t;

class SSD1306 {
public:

  SSD1306(I2c_Bridge& bridge);

  void enable();
  void disable();

  void clear();
  void updateDisplay();

  void setFont(const FontBridge* font);
  void drawString(int x, int y, const std::string& strUser);
  unsigned int getStringWidth(const std::string& text) const;

  unsigned int width() const { return 128;}
  unsigned int height() const { return 64;}

  void drawXbm(int x, int y, const IconBridge& icon);
  void setPixel(int x, int y);
private:
  using ScreenBuffer = std::array<uint8_t, 1 + (128 * 64) / 8>;

  I2c_Bridge&   bridge;
  ScreenBuffer screen;
  const FontBridge* font;

  void sendCmd(SSD1306Cmd cmd);
  void sendCmd(SSD1306Cmd cmd, uint8_t param);
  void sendCmdStream(std::vector<uint8_t>&);

  void drawInternal(int x, int y, const FontCharacter& character);
};
