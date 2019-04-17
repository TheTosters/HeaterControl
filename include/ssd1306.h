#pragma once

extern "C" {
#include "boards.h"
}

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

  SSD1306(const SSD1306&) = delete;
  SSD1306& operator=(const SSD1306&) = delete;

  void enable();
  void disable();

  void clear();
  void updateDisplay();

  void setFont(const FontBridge* font);
  void drawString(int x, int y, const std::string& strUser);
  unsigned int getStringWidth(const std::string& text) const;

  unsigned int width() const { return WIDTH;}
  unsigned int height() const { return HEIGHT;}

  void drawXbm(int x, int y, const IconBridge& icon);
  void setPixel(int x, int y);
private:
  static constexpr unsigned int WIDTH = 128;
  static constexpr unsigned int HEIGHT = 64;

  using ScreenBuffer = std::array<uint8_t, 1 + (WIDTH * HEIGHT) / 8>;
  static constexpr I2c_Bridge::I2cAddress SSD_ADDR{CONFIG_SSD1306_I2C_ADDRESS};

  I2c_Bridge&   bridge;
  ScreenBuffer screen;
  const FontBridge* font;

  void sendCmd(SSD1306Cmd cmd);
  void sendCmd(SSD1306Cmd cmd, uint8_t param);
  void sendCmdStream(const std::vector<uint8_t>&);

  void drawInternal(int x, int y, const FontCharacter& character);
};
