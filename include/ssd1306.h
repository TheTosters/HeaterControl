#pragma once

#include <cinttypes>
#include <vector>
#include <array>
#include <string>
#include "i2c_bridge.h"
#include "font_bridge.h"

enum SSD1306Cmd : uint8_t;

class SSD1306 {
public:

  SSD1306(I2c_Bridge& bridge);

  void begin();
  void end();

  void clear();
  void updateDisplay();

  void setFont(FontBridge* font);
  void drawString(int x, int y, const std::string& strUser);
  unsigned int getStringWidth(const std::string& text) const;

  unsigned int width() const { return 128;}
  unsigned int height() const { return 64;}

  void drawXbm(int x, int y, int width, int height, const uint8_t* xbmData);
  void setPixel(int x, int y);
private:
  using ScreenBuffer = std::array<uint8_t, 1 + (128 * 64) / 8>;

  I2c_Bridge&   bridge;
  ScreenBuffer screen;
  FontBridge* font;

  void sendCmd(SSD1306Cmd cmd);
  void sendCmd(SSD1306Cmd cmd, uint8_t param);
  void sendCmdStream(std::vector<uint8_t>&);

  void drawInternal(int x, int y, const FontCharacter& character);
};
