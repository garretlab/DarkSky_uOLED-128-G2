#include "Arduino.h"
#include "goldelox.h"

/*
   Constructor
*/
GOLDELOX::GOLDELOX(Stream *port) {
  serialPort = port;
}

/*
    Public methods
*/
void GOLDELOX::begin() {
  setDisplayModel();
}

/*
   Text Commands
*/
int GOLDELOX::moveCursor(uint16_t x, uint16_t y) {
  sendWord(GOLDELOX_CMD_MOVE_CURSOR);
  sendWord(y);
  sendWord(x);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::putCharacter(char character) {
  sendWord(GOLDELOX_CMD_PUT_CHARACTER);
  sendWord(character);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::putString(const char *string) {
  int length = strlen(string);

  sendWord(GOLDELOX_CMD_PUT_STRING);
  for (int i = 0; i < length; i++) {
    sendByte(string[i]);
  }
  sendByte(0x00);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::characterWidth(char character, uint16_t *width) {
  int result;

  sendWord(GOLDELOX_CMD_CHARACTER_WIDTH);
  sendByte(character);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, width));
}

int GOLDELOX::characterHeight(char character, uint16_t *height) {
  int result;

  sendWord(GOLDELOX_CMD_CHARACTER_HEIGHT);
  sendByte(character);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, height));
}

int GOLDELOX::textForegroundColor(uint16_t color) {
  sendWord(GOLDELOX_CMD_TEXT_FOREGROUND_COLOR);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textBackgroundColor(uint16_t color) {
  sendWord(GOLDELOX_CMD_TEXT_BACKGROUND_COLOR);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textWidth(uint16_t multiplier) {
  sendWord(GOLDELOX_CMD_TEXT_WIDTH);
  sendWord(multiplier);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textHeight(uint16_t multiplier) {
  sendWord(GOLDELOX_CMD_TEXT_HEIGHT);
  sendWord(multiplier);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textXGap(uint16_t pixelcount) {
  sendWord(GOLDELOX_CMD_TEXT_X_GAP);
  sendWord(pixelcount);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textYGap(uint16_t pixelcount) {
  sendWord(GOLDELOX_CMD_TEXT_Y_GAP);
  sendWord(pixelcount);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textBold(uint16_t mode) {
  sendWord(GOLDELOX_CMD_TEXT_BOLD);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textInverse(uint16_t mode) {
  sendWord(GOLDELOX_CMD_TEXT_INVERSE);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textItalic(uint16_t mode) {
  sendWord(GOLDELOX_CMD_TEXT_ITALIC);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textOpacity(uint16_t mode) {
  sendWord(GOLDELOX_CMD_TEXT_OPACITY);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textUnderline(uint16_t mode) {
  sendWord(GOLDELOX_CMD_TEXT_UNDERLINE);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::textAttributes(uint16_t value) {
  sendWord(GOLDELOX_CMD_TEXT_ATTRIBUTES);
  sendWord(value);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::setTextParameters(uint16_t function, uint16_t value) {
  sendWord(GOLDELOC_CMD_SET_TEXT_PARAMETERS);
  sendWord(function);
  sendWord(value);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

/*
   Graphics Commands
*/
int GOLDELOX::clearScreen() {
  sendWord(GOLDELOX_CMD_CLEAR_SCREEN);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::changeColor(uint16_t oldColor, uint16_t newColor) {
  sendWord(GOLDELOX_CMD_CHANGE_COLOR);
  sendWord(oldColor);
  sendWord(newColor);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_CIRCLE);
  sendWord(x);
  sendWord(y);
  sendWord(rad);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawFilledCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_FILLED_CIRCLE);
  sendWord(x);
  sendWord(y);
  sendWord(rad);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_LINE);
  sendWord(x1);
  sendWord(y1);
  sendWord(x2);
  sendWord(y2);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_RECTANGLE);
  sendWord(x1);
  sendWord(y1);
  sendWord(x2);
  sendWord(y2);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_FILLED_RECTANGLE);
  sendWord(x1);
  sendWord(y1);
  sendWord(x2);
  sendWord(y2);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawPolyline(uint16_t n, uint16_t *xArray, uint16_t *yArray, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_POLYLINE);
  sendWord(n);

  for (int i = 0; i < n; i++) {
    sendWord(xArray[i]);
  }

  for (int i = 0; i < n; i++) {
    sendWord(yArray[i]);
  }

  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawPolygon(uint16_t n, uint16_t *xArray, uint16_t *yArray, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_POLYGON);
  sendWord(n);

  for (int i = 0; i < n; i++) {
    sendWord(xArray[i]);
  }

  for (int i = 0; i < n; i++) {
    sendWord(yArray[i]);
  }

  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color) {
  sendWord(GOLDELOX_CMD_DRAW_TRIANGLE);
  sendWord(x1);
  sendWord(y1);
  sendWord(x2);
  sendWord(y2);
  sendWord(x3);
  sendWord(y3);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::calculateOrbit(uint16_t angle, uint16_t distance, uint16_t *dist) {
  sendWord(GOLDELOX_CMD_CALCULATE_ORBIT);
  sendWord(angle);
  sendWord(distance);

  return getResponseWords(GOLDELOX_RESPONSE_ACK, 2, dist);
}

int GOLDELOX::putPixel(uint16_t x, uint16_t y, uint16_t color) {
  sendWord(GOLDELOX_CMD_PUT_PIXEL);
  sendWord(x);
  sendWord(y);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::readPixel(uint16_t x, uint16_t y, uint16_t *color) {
  sendWord(GOLDELOX_CMD_READ_PIXEL);
  sendWord(x);
  sendWord(y);

  getResponseWord(GOLDELOX_RESPONSE_ACK, color);
  return true;
}

int GOLDELOX::moveOrigin(uint16_t xpos, uint16_t ypos) {
  sendWord(GOLDELOX_CMD_MOVE_ORIGIN);
  sendWord(xpos);
  sendWord(ypos);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawLineAndMoveOrigin(uint16_t xpos, uint16_t ypos) {
  sendWord(GOLDELOX_CMD_DRAW_LINE_AND_MOVE_ORIGIN);
  sendWord(xpos);
  sendWord(ypos);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::clipping(uint16_t value) {
  sendWord(GOLDELOX_CMD_CLIPPING);
  sendWord(value);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::setClipWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  sendWord(GOLDELOX_CMD_SET_CLIP_WINDOW);
  sendWord(x1);
  sendWord(y1);
  sendWord(x2);
  sendWord(y2);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::extendClipRegion() {
  sendWord(GOLDELOX_CMD_EXTEND_CLIP_REGION);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::backgroundColor(uint16_t color) {
  sendWord(GOLDELOX_CMD_BACKGROUND_COLOR);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::outlineColor(uint16_t color) {
  sendWord(GOLDELOX_CMD_OUTLINE_COLOR);
  sendWord(color);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::contrast(uint16_t contrast) {
  sendWord(GOLDELOX_CMD_CONTRAST);
  sendWord(contrast);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::frameDelay(uint16_t msec) {
  sendWord(GOLDELOX_CMD_FRAME_DELAY);
  sendWord(msec);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::linePattern(uint16_t pattern) {
  sendWord(GOLDELOX_CMD_LINE_PATTERN);
  sendWord(pattern);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::screenMode(uint16_t mode) {
  sendWord(GOLDELOX_CMD_SCREEN_MODE);
  sendWord(mode);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::setGraphicsParameters(uint16_t function, uint16_t value) {
  sendWord(GOLDELOX_CMD_SET_GRAPHICS_PARAMETERS);
  sendWord(function);
  sendWord(value);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::drawPattern(uint16_t x, uint16_t y, uint8_t nX, int8_t nY, uint8_t *pattern, uint16_t color) {
  uint8_t *p = pattern;
  uint8_t curBit = 7;

  for (int yy = 0; yy < nY; yy++) {
    for (int xx = 0; xx < nX; xx++) {
      if (*p & (1 << curBit)) {
        putPixel(x + xx, y + yy, color);
      }
      if (curBit == 0) {
        p++;
        curBit = 7;
      } else {
        curBit--;
      }
    }
  }
  return true;
}

/*
   Media Commands(SD/SDHC Memory Cards)
*/
int GOLDELOX::mediaInit(uint16_t *value) {
  sendWord(GOLDELOX_CMD_MEDIA_INIT);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, value));
}

int GOLDELOX::setByteAddress(uint32_t address) {
  sendWord(GOLDELOX_CMD_SET_BYTE_ADDRESS);
  sendWord(address >> 16);
  sendWord(address & 0xffff);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::setSectorAddress(uint32_t address) {
  sendWord(GOLDELOX_CMD_SET_SECTOR_ADDRESS);
  sendWord(address >> 16);
  sendWord(address & 0xffff);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::readByte(uint8_t *value) {
  sendWord(GOLDELOX_CMD_READ_BYTE);

  return (getResponseByte(GOLDELOX_RESPONSE_ACK, value));
}

int GOLDELOX::readWord(uint16_t *value) {
  sendWord(GOLDELOX_CMD_READ_WORD);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, value));
}

int GOLDELOX::writeByte(uint8_t value, uint16_t *status) {
  sendWord(GOLDELOX_CMD_WRITE_BYTE);
  sendByte(0x00);
  sendByte(value);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, status));
}

int GOLDELOX::writeWord(uint16_t value, uint16_t *status) {
  sendWord(GOLDELOX_CMD_WRITE_BYTE);
  sendWord(value);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, status));
}

int GOLDELOX::flushMedia(uint16_t *status) {
  sendWord(GOLDELOX_CMD_FLUSH_MEDIA);

  return (getResponseWord(GOLDELOX_RESPONSE_ACK, status));
}

int GOLDELOX::displayImageRaw(uint16_t x, uint16_t y) {
  sendWord(GOLDELOX_CMD_DISPLAY_IMAGE_RAW);
  sendWord(x);
  sendWord(y);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::displayVideoRaw(uint16_t x, uint16_t y) {
  sendWord(GOLDELOX_CMD_DISPLAY_VIDEO_RAW);
  sendWord(x);
  sendWord(y);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::displayVideoFrameRaw(uint16_t x, uint16_t y, uint16_t frameNumber) {
  sendWord(GOLDELOX_CMD_DISPLAY_VIDEO_RAW);
  sendWord(x);
  sendWord(y);
  sendWord(frameNumber);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

/*
   Sound and Tune Commands
*/
int GOLDELOX::beep(uint16_t note, uint16_t duration) {
  sendWord(GOLDELOX_CMD_BEEP);
  sendWord(note);
  sendWord(duration);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

/*
   Serial Communications Commands
*/
int GOLDELOX::setBaudRate(int baudRate, HardwareSerial *port) {
  int i = -1;
  int retval;

  while (baudRateIndex[++i].index) {
    if (baudRateIndex[i].baudRate == baudRate) {
      sendWord(GOLDELOX_SET_BAUD_RATE);
      sendWord(baudRateIndex[i].index);
      delay(100);
      port->end();
      port->begin(baudRate);
      // retval = getResponse(GOLDELOX_RESPONSE_ACK);
      //return retval;
    }
  }
  return true;
}

/*
   System Commands
*/
int GOLDELOX::getDisplayModel(char *string) {
  sendWord(GOLDELOX_CMD_GET_DISPLAY_MODEL);

  return getResponseString(GOLDELOX_RESPONSE_ACK, string);
}

int GOLDELOX::screenSaverTimeout(uint16_t timeout) {
  sendWord(GOLDELOX_CMD_SCREEN_SAVER_TIMEOUT);
  sendWord(timeout);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

int GOLDELOX::screenSaverSpeed(uint16_t speed) {
  sendWord(GOLDELOX_CMD_SCREEN_SAVER_SPEED);
  sendWord(speed);

  return getResponse(GOLDELOX_RESPONSE_ACK);
}

/*
    Private methods
*/
int GOLDELOX::sendByte(uint8_t command) {
  serialPort->write((command));
  return true;
}

int GOLDELOX::sendWord(uint16_t command) {
  serialPort->write((command >> 8));
  serialPort->write((command & 0xff));
  return true;
}

int GOLDELOX::getResponse(uint8_t response) {
  while (serialPort->available() <= 0) {
    delay(2);
  }

  return (response == serialPort->read()) ? true : false;
}

int GOLDELOX::getResponseByte(uint8_t response, uint8_t *value) {
  union {
    char buffer[2];
    uint16_t value;
  } buffer;
  int result;

  if ((result = getResponse(response))) {
    while (serialPort->available() < 2) {
      delay(2);
    }
    buffer.buffer[1] = serialPort->read();
    buffer.buffer[0] = serialPort->read();
  }
  *value = buffer.value & 0x0f;

  return result;
}

int GOLDELOX::getResponseWord(uint8_t response, uint16_t *value) {
  union {
    char buffer[2];
    uint16_t value;
  } buffer;
  int result;

  if ((result = getResponse(response))) {
    while (serialPort->available() < 2) {
      delay(2);
    }
    buffer.buffer[1] = serialPort->read();
    buffer.buffer[0] = serialPort->read();
  }
  *value = buffer.value;

  return result;
}

int GOLDELOX::getResponseWords(uint8_t response, uint16_t n, uint16_t *value) {
  union {
    char buffer[2];
    uint16_t value;
  } buffer;
  int result;
  int nbytes = 2 * n;

  if ((result = getResponse(response))) {
    while (serialPort->available() < nbytes) {
      delay(2);
    }

    for (int i = 0; i < nbytes; i++) {
      buffer.buffer[1] = serialPort->read();
      buffer.buffer[0] = serialPort->read();

      value[i] = buffer.value;
    }
  }

  return result;
}

int GOLDELOX::getResponseString(uint8_t response, char *string) {
  uint16_t value;
  int result;

  if ((result = getResponseWord(response, &value))) {
    serialPort->readBytes(string, value);
    string[value] = '\0';
  }

  return result;
}

int GOLDELOX::setDisplayModel() {
  int result = getDisplayModel(displayModel);

  return result;
}
