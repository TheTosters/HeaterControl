#include "i2c_bridge.h"
#include "ssd1306.h"

extern "C" {
#include "nrf_log.h"
#include "boards.h"
}

#include <inttypes.h>
#include <algorithm>

//partially based on code from:
//https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master

enum SSD1306Cmd : uint8_t {
  SET_CONTRAST = 0x81,
  DISPLAY_ALL_ON_RESUME = 0xA4,
  DISPLAY_ALL_ON = 0xA5,
  NORMAL_DISPLAY = 0xA6,
  INVERT_DISPLAY = 0xA7,
  DISPLAY_OFF = 0xAE,
  DISPLAY_ON = 0xAF,

  SET_DISPLAY_OFFSET = 0xD3,
  SET_COMPINS = 0xDA,

  SET_VCOM_DETECT = 0xDB,

  SET_DISPLAY_CLOCK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,

  SET_MULTIPLEX = 0xA8,

  SET_LOW_COLUMN = 0x00,
  SET_HIGH_COLUMN = 0x10,

  SET_START_LINE = 0x40,

  MEMORY_MODE = 0x20,
  COLUMN_ADDR = 0x21,
  PAGE_ADDR = 0x22,

  COM_SCAN_INC = 0xC0,
  COM_SCAN_DEC = 0xC8,

  SEG_REMAP = 0xA0,

  CHARGE_PUMP = 0x8D,

  EXTERNAL_VCC = 0x1,
  SWITCH_CAP_VCC = 0x2,

// Scrolling #defines
  ACTIVATE_SCROLL = 0x2F,
  DEACTIVATE_SCROLL = 0x2E,
  SET_VERTICAL_SCROLL_AREA = 0xA3,
  RIGHT_HORIZONTAL_SCROLL = 0x26,
  LEFT_HORIZONTAL_SCROLL = 0x27,
  VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
  VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A,
};

SSD1306::SSD1306(I2c_Bridge& bridge)
 : bridge(bridge),
   font(nullptr) {
}

void SSD1306::end() {
  sendCmd(DISPLAY_OFF);
}

void SSD1306::begin() {
  std::vector<uint8_t> cmdStream({
    0,
    DISPLAY_OFF,
    SET_DISPLAY_CLOCK_DIV, 0x80, //F0 Increase speed of the display max ~96Hz
    SET_MULTIPLEX, 64 - 1,    //height -1
    SET_DISPLAY_OFFSET, 0,    //no offset
    SET_START_LINE | 0x0,     //line #0
    CHARGE_PUMP, 0x14,        //SSD1306_EXTERNALVCC -> 0x10
    MEMORY_MODE, 0x00,        //0x0 act like ks0108
    SEG_REMAP,                //should be "| 0x1" ?
    COM_SCAN_INC,
    SET_COMPINS, 0x12,        //GEOMETRY_128_64
    SET_CONTRAST, 0xCF,       //SSD1306_EXTERNALVCC -> 0x9F
    SET_PRECHARGE, 0xF1,      //SSD1306_EXTERNALVCC -> 0x22
    SET_VCOM_DETECT, 0x40,
    DISPLAY_ALL_ON_RESUME,
    NORMAL_DISPLAY, 0x2e, // stop scroll
    DISPLAY_ON
  });

  sendCmdStream(cmdStream);
}

void SSD1306::clear() {
  //TODO: Use easyDma?
  std::fill(screen.begin(), screen.end(), 0);
}

void SSD1306::updateDisplay() {
  std::vector<uint8_t> cmdStream({
    0,
    // Column start address (0 = reset), Column end address (127 = reset)
    COLUMN_ADDR, 0, 127,
    PAGE_ADDR, 0, 7,  // Page start address (0 = reset), set 7 -> 64 lines
  });
  sendCmdStream(cmdStream);

  constexpr uint8_t chunkSize = 254;
  ScreenBuffer::iterator offset = std::next(screen.begin());
  //TODO: reimplement to use easyDma + PPI
  do {
    uint8_t size = (screen.end() - offset) >= chunkSize ?
        chunkSize : screen.end() - offset;

    offset = std::prev(offset);

    uint8_t tmp = *offset;
    *offset = SET_START_LINE;

    bridge.send(CONFIG_SSD1306_I2C_ADDRESS, &(*offset), size + 1);
    *offset = tmp;

    offset = std::next(offset, size + 1);
  } while (offset < screen.end());
}

void SSD1306::sendCmd(SSD1306Cmd cmd) {
  bridge.send(CONFIG_SSD1306_I2C_ADDRESS, static_cast<uint8_t>(0), cmd);
}

void SSD1306::sendCmd(SSD1306Cmd cmd, uint8_t param) {
  bridge.send(CONFIG_SSD1306_I2C_ADDRESS, 0, cmd, param);
}

void SSD1306::sendCmdStream(std::vector<uint8_t>& cmds) {
  bridge.send(CONFIG_SSD1306_I2C_ADDRESS, cmds.data(),
      static_cast<uint8_t>(cmds.size()));
}

void SSD1306::drawInternal(int x, int y, const FontCharacter& character) {

  // fast ceil(height / 8.0)
  //int rasterHeight = 1 + ((font->getFontHeight() - 1) / 8);
  int rasterHeight = 1 + ((font->getFontHeight() - 1) >> 3);
  int yOffset = y & 7;
  unsigned int bytesInData = character.charByteSize() == 0
      ? character.charWidth() * rasterHeight : character.charByteSize();

  int initY = y;
  int initYOffset = yOffset;

  for (unsigned int i = 0; i < bytesInData; i++) {

    // Reset if next horizontal drawing phase is started.
    if (i % rasterHeight == 0) {
      y = initY;
      yOffset = initYOffset;
    }

    uint8_t currentByte = character.getDataByte(i);

    int xPos = x + (i / rasterHeight);
//    int yPos = ((y / 8) + (i % rasterHeight)) * this->width();
    int yPos = ((y >> 3) + (i % rasterHeight)) * this->width();

    int dataPos = xPos + yPos;
    const int displayBufferSize = screen.size() - 1;
    if (dataPos >= 0 and dataPos < displayBufferSize and
        xPos >= 0 and xPos < this->width()) {

      if (yOffset >= 0) {
        screen[1 + dataPos] |= currentByte << yOffset;
        if (dataPos < (displayBufferSize - this->width())) {
          screen[1 + dataPos + this->width()] |= currentByte >> (8 - yOffset);
        }
      } else {
        // Make new offset position
        yOffset = -yOffset;
        screen[1 + dataPos] |= currentByte >> yOffset;

        // Prepare for next iteration by moving one block up
        y -= 8;

        // and setting the new yOffset
        yOffset = 8 - yOffset;
      }
    }
  }
}

void SSD1306::drawString(int x, int y, const std::string& text) {
  if (font == nullptr) {
    NRF_LOG_ERROR("Set Font first!");
    return;
  }

  unsigned int textWidth = getStringWidth(text);

  // Don't draw anything if it is not on the screen.
  if (x + textWidth  < 0 or x > this->width() or
      y + font->getFontHeight() < 0 or y > this->height() ) {
    return;
  }

  for(const char& code : text) {
    if (code >= font->getFirstChar()) {
      FontCharacter character = font->getCharacter(code);

      if (character.isDrawable()) {
        // Get the position of the char data
        drawInternal(x, y, character);
      }

      x += character.charWidth();
    }
  }
}

unsigned int SSD1306::getStringWidth(const std::string& text) const {
  unsigned int stringWidth = 0;
  for(const char& c : text) {
    stringWidth += font->getFontWidth(c);
  }

  return stringWidth;
}

void SSD1306::setFont(const FontBridge* font) {
  this->font = font;
}

void SSD1306::setPixel(int x, int y) {
  if (x >= 0 && x < this->width() && y >= 0 && y < this->height()) {
    int dataPos = x + (y / 8) * this->width();
    screen[1 + dataPos] |=  (1 << (y & 7));
  }
}

void SSD1306::drawXbm(int xx, int yy, const IconBridge& icon) {

  int widthInXbm = (icon.width + 7) / 8;
  uint8_t data = 0;

  for(int y = 0; y < icon.height; y++) {
    for(int x = 0; x < icon.width; x++ ) {
      if (x & 7) {
        data >>= 1; // Move a bit

      } else {  // Read new data every 8 bit
        data = *(icon.data + (x / 8) + y * widthInXbm);
      }
      // if there is a bit draw it
      if (data & 0x01) {
        setPixel(xx + x, yy + y);
      }
    }
  }
}
