#pragma once

#include <cinttypes>

typedef struct __attribute__((packed)) {
  uint8_t msb;
  uint8_t lsb;
  uint8_t size;
  uint8_t width;
} CharDesc;

class FontCharacter {
public:
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
  FontBridge(const uint8_t* fontBuffer)
    : fontBuffer(fontBuffer),
      firstChar( *(fontBuffer + FIRST_CHAR_POS) ){
  }

  unsigned int getFontWidth(uint8_t character) const {
    return charDescr(character)->width;
  }

  unsigned int getFontHeight() const {
    return *(fontBuffer + HEIGHT_POS);
  }

  unsigned int getFirstChar() const {
    return firstChar;
  }

  FontCharacter getCharacter(uint8_t character) const {
    const CharDesc* cd = charDescr(character);
    unsigned int sizeOfJumpTable = firstChar * JUMPTABLE_BYTES;
    unsigned int charDataPosition = JUMPTABLE_START + sizeOfJumpTable +
                ((cd->msb << 8) + cd->lsb);
    const uint8_t* ptr = fontBuffer + charDataPosition;
    FontCharacter result(cd, ptr);
    return result;
  }

private:
  enum JumpTable{
    JUMPTABLE_BYTES = 4,
    JUMPTABLE_START = 4,
  };

  enum FontSpec {
    WIDTH_POS = 0,
    HEIGHT_POS = 1,
    FIRST_CHAR_POS = 2,
    CHAR_NUM_POS = 3,
  };

  const uint8_t* fontBuffer;
  const unsigned int firstChar;

  unsigned int charDescrOffset(uint8_t character) const {
    return JUMPTABLE_START + (character - firstChar) * JUMPTABLE_BYTES;
  }

  const CharDesc* charDescr(uint8_t character) const {
    return reinterpret_cast<const CharDesc*>(fontBuffer + charDescrOffset(character));
  }
};
