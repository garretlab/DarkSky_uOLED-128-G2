#ifndef GOLDELOX_H
#define GOLDELOX_H

#include <stdint.h>
class Stream;

#define GOLDELOX_CMD_MOVE_CURSOR                0xffe4
#define GOLDELOX_CMD_PUT_CHARACTER              0xfffe
#define GOLDELOX_CMD_PUT_STRING                 0x0006
#define GOLDELOX_CMD_CHARACTER_WIDTH            0x0002
#define GOLDELOX_CMD_CHARACTER_HEIGHT           0x0001
#define GOLDELOX_CMD_TEXT_FOREGROUND_COLOR      0xff7f
#define GOLDELOX_CMD_TEXT_BACKGROUND_COLOR      0xff7e
#define GOLDELOX_CMD_SET_FONT                   0xff7d
#define GOLDELOX_CMD_TEXT_WIDTH                 0xff7c
#define GOLDELOX_CMD_TEXT_HEIGHT                0xff7b
#define GOLDELOX_CMD_TEXT_X_GAP                 0xff7a
#define GOLDELOX_CMD_TEXT_Y_GAP                 0xff79
#define GOLDELOX_CMD_TEXT_BOLD                  0xff76
#define GOLDELOX_CMD_TEXT_INVERSE               0xff74
#define GOLDELOX_CMD_TEXT_ITALIC                0xff75
#define GOLDELOX_CMD_TEXT_OPACITY               0xff77
#define GOLDELOX_CMD_TEXT_UNDERLINE             0xff73
#define GOLDELOX_CMD_TEXT_ATTRIBUTES            0xff72
#define GOLDELOC_CMD_SET_TEXT_PARAMETERS        0xffe3

#define GOLDELOX_CMD_CLEAR_SCREEN               0xffd7
#define GOLDELOX_CMD_CHANGE_COLOR               0xffbe
#define GOLDELOX_CMD_DRAW_CIRCLE                0xffcd
#define GOLDELOX_CMD_DRAW_FILLED_CIRCLE         0xffcc
#define GOLDELOX_CMD_DRAW_LINE                  0xffd2
#define GOLDELOX_CMD_DRAW_RECTANGLE             0xffcf
#define GOLDELOX_CMD_DRAW_FILLED_RECTANGLE      0xffce
#define GOLDELOX_CMD_DRAW_POLYLINE              0x0005
#define GOLDELOX_CMD_DRAW_POLYGON               0x0004
#define GOLDELOX_CMD_DRAW_TRIANGLE              0xffc9
#define GOLDELOX_CMD_CALCULATE_ORBIT            0x0003
#define GOLDELOX_CMD_PUT_PIXEL                  0xffcb
#define GOLDELOX_CMD_READ_PIXEL                 0xffca
#define GOLDELOX_CMD_MOVE_ORIGIN                0xffd6
#define GOLDELOX_CMD_DRAW_LINE_AND_MOVE_ORIGIN  0xffd4
#define GOLDELOX_CMD_CLIPPING                   0xff6c
#define GOLDELOX_CMD_SET_CLIP_WINDOW            0xffbf
#define GOLDELOX_CMD_EXTEND_CLIP_REGION         0xffbc
#define GOLDELOX_CMD_BACKGROUND_COLOR           0xff6e
#define GOLDELOX_CMD_OUTLINE_COLOR              0xff67
#define GOLDELOX_CMD_CONTRAST                   0xff66
#define GOLDELOX_CMD_FRAME_DELAY                0xff69
#define GOLDELOX_CMD_LINE_PATTERN               0xff65
#define GOLDELOX_CMD_SCREEN_MODE                0xff68
#define GOLDELOX_CMD_SET_GRAPHICS_PARAMETERS    0xffd8

#define GOLDELOX_CMD_MEDIA_INIT                 0xffb1
#define GOLDELOX_CMD_SET_BYTE_ADDRESS           0xffb9
#define GOLDELOX_CMD_SET_SECTOR_ADDRESS         0xffb8
#define GOLDELOX_CMD_READ_BYTE                  0xffb7
#define GOLDELOX_CMD_READ_WORD                  0xffb6
#define GOLDELOX_CMD_WRITE_BYTE                 0xffb5
#define GOLDELOX_CMD_WRITE_WORD                 0xffb4
#define GOLDELOX_CMD_FLUSH_MEDIA                0xffb2
#define GOLDELOX_CMD_DISPLAY_IMAGE_RAW          0xffb3
#define GOLDELOX_CMD_DISPLAY_VIDEO_RAW          0xffbb
#define GOLDELOX_CMD_DISPLAY_VIDEO_FRAME_RAW    0xffba

#define GOLDELOX_CMD_BEEP                       0xffda

#define GOLDELOX_CMD_GET_DISPLAY_MODEL          0x0007
#define GOLDELOX_CMD_SCREEN_SAVER_TIMEOUT       0x000c
#define GOLDELOX_CMD_SCREEN_SAVER_SPEED         0x000d

#define GOLDELOX_RESPONSE_ACK                   0x06

#define GOLDELOX_SYSTEM_FONT                    0x00
#define GOLDELOX_MEDIA_FONT                     0x07

#define GOLDELOX_TEXT_BOLD                      0x10
#define GOLDELOX_TEXT_ITALIC                    0x20
#define GOLDELOX_TEXT_INVERSE                   0x40
#define GOLDELOX_TEXT_UNDERLINED                0x80

#define GOLDELOX_TEXT_PRINT_DELAY               0x07

#define GOLDELOX_CLIPPING_DISABLED              0x00
#define GOLDELOX_CLIPPING_ENABLED               0x01

#define GOLDELOX_LANDSCAPE                      0x01
#define GOLDELOX_LANDSCAPE_REVERSE              0x02
#define GOLDELOX_PORTRAIT                       0x03
#define GOLDELOX_PORTRAIT_REVERSE               0x04

#define GOLDELOX_PEN_SIZE                       0x00
#define GOLDELOX_SOLID                          0x00
#define GOLDELOX_OUTLINE                        0x01

#define GOLDELOX_OBJECT_COLOR                   0x02

class GOLDELOX {
public:
/*
 * Constructor
 */
  GOLDELOX(Stream  *port);

  char displayModel[20];

/*
 *  Public methods
 */
  void begin();
 
/*
 * Text Commands
 */
  int moveCursor(uint16_t x, uint16_t y);
  int putCharacter(char character);
  int putString(char *string);
  int characterWidth(char character, uint16_t *width);
  int characterHeight(char character, uint16_t *height);
  int textForegroundColor(uint16_t color);
  int textBackgroundColor(uint16_t color);
  int textWidth(uint16_t multiplier);
  int textHeight(uint16_t multiplier);
  int textXGap(uint16_t pixelcount);
  int textYGap(uint16_t pixelcount);
  int textBold(uint16_t mode);
  int textInverse(uint16_t mode);
  int textItalic(uint16_t mode);
  int textOpacity(uint16_t mode);
  int textUnderline(uint16_t mode);
  int textAttributes(uint16_t value);
  int setTextParameters(uint16_t function, uint16_t value);
 
/*
 * Graphics Commands
 */
  int clearScreen();
  int changeColor(uint16_t oldColor, uint16_t newColor); 
  int drawCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color);
  int drawFilledCircle(uint16_t x, uint16_t y, uint16_t rad, uint16_t color);
  int drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  int drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  int drawFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  int drawPolyline(uint16_t n, uint16_t *xArray, uint16_t *yArray, uint16_t color);
  int drawPolygon(uint16_t n, uint16_t *xArray, uint16_t *yArray, uint16_t color);
  int drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
  int calculateOrbit(uint16_t angle, uint16_t distance, uint16_t *dist);
  int putPixel(uint16_t x, uint16_t y, uint16_t color);
  int readPixel(uint16_t x, uint16_t y, uint16_t *color);
  int moveOrigin(uint16_t xpos, uint16_t ypos);
  int drawLineAndMoveOrigin(uint16_t xpos, uint16_t ypos);
  int clipping(uint16_t value);
  int setClipWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  int extendClipRegion();
  int backgroundColor(uint16_t color);
  int outlineColor(uint16_t color);
  int contrast(uint16_t contrast);
  int frameDelay(uint16_t msec);
  int linePattern(uint16_t pattern);
  int screenMode(uint16_t mode);
  int setGraphicsParameters(uint16_t function, uint16_t value);
  
/*
 * Media Commands(SD/SDHC Memory Cards)
 */
  int mediaInit(uint16_t *value);
  int setByteAddress(uint32_t address);
  int setSectorAddress(uint32_t address);
  int readByte(uint8_t *value);
  int readWord(uint16_t *value);
  int writeByte(uint8_t value, uint16_t *status);
  int writeWord(uint16_t value, uint16_t *status);
  int flushMedia(uint16_t *status);
  int displayImageRaw(uint16_t x, uint16_t y);
  int displayVideoRaw(uint16_t x, uint16_t y);
  int displayVideoFrameRaw(uint16_t x, uint16_t y, uint16_t frameNumber);

/*
 * Sound and Tune Commands
 */
  int beep(uint16_t note, uint16_t duration);
  
/*
 * System Commands
 */
  int getDisplayModel(char *string);
  int screenSaverTimeout(uint16_t timeout);
  int screenSaverSpeed(uint16_t speed);
  
private:
  Stream *serialPort;
/*
 *  Private methods
 */
  int getResponse(uint8_t response);
  int getResponseByte(uint8_t response, uint8_t *value);
  int getResponseWord(uint8_t response, uint16_t *value);
  int getResponseWords(uint8_t response, uint16_t n, uint16_t *value);
  int getResponseString(uint8_t response, char *string);
  int sendByte(uint8_t command);
  int sendWord(uint16_t command);
  int setDisplayModel();
};

#endif /* GOLDELOX_H */

