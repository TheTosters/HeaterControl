#pragma once

#include <cinttypes>
#include <initializer_list>

struct __attribute__((packed)) CharDesc {
  uint8_t msb;
  uint8_t lsb;
  uint8_t size;
  uint8_t width;
};

class FontCharacter {
public:
  FontCharacter(nullptr_t, nullptr_t) = delete;
  FontCharacter(nullptr_t, const uint8_t* charDataPtr) = delete;
  FontCharacter(const CharDesc* descr, nullptr_t) = delete;

  FontCharacter(const CharDesc* descr, const uint8_t* charDataPtr)
    : cd(descr), charPtr(charDataPtr) {}

  bool isDrawable() {
    //not(cd->msb == 255 and cd->lsb == 255);
    return cd->msb != 255 or cd->lsb != 255;
  }

  unsigned int charByteSize() const {
    return cd->size;
  }

  unsigned int charWidth() const {
    return cd->width;
  }

  uint8_t getDataByte(unsigned int index) const {
    return *(charPtr + index);
  }

private:
  const CharDesc* cd;
  const uint8_t* charPtr;
};

class FontBridge {
public:
  FontBridge(const uint8_t* data) :
    fontBuffer(data),
    fontsDescr( reinterpret_cast<const FontsDesc*>(data)) {
  }

  unsigned int getFontWidth(uint8_t character) const {
    return charDescr(character)->width;
  }

  int getFontHeight() const {
    return fontsDescr->height;
  }

  unsigned int getFirstChar() const {
    return fontsDescr->firstChar;
  }

  FontCharacter getCharacter(uint8_t character) const {
    const CharDesc* cd = charDescr(character);
    unsigned int sizeOfJumpTable = fontsDescr->numberOfChars * sizeof(CharDesc);
    unsigned int charDataPosition = sizeof(FontsDesc) + sizeOfJumpTable +
                ((cd->msb << 8) + cd->lsb);
    const uint8_t* ptr = fontBuffer + charDataPosition;
    FontCharacter result(cd, ptr);
    return result;
  }

private:
  typedef struct __attribute__((packed)) {
    uint8_t width;
    uint8_t height;
    uint8_t firstChar;
    uint8_t numberOfChars;
  } FontsDesc;

  const uint8_t* fontBuffer;
  const FontsDesc* fontsDescr;

  unsigned int charDescrOffset(uint8_t character) const {
    return sizeof(FontsDesc) + (character - fontsDescr->firstChar) * sizeof(CharDesc);
  }

  const CharDesc* charDescr(uint8_t character) const {
    return reinterpret_cast<const CharDesc*>(fontBuffer + charDescrOffset(character));
  }
};
