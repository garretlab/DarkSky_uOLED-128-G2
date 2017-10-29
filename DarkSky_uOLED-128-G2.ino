#include <WiFi.h>
#include "goldelox.h"
#include "JsonListener.h"
#include "DarkskyParser.h"

DarkskyParser dsParser;

HardwareSerial Serial2(2);
GOLDELOX oled(&Serial2);

const char *ssid = "YOURSSID";
const char *password = "YOURPASSWORD";

const char *apiKey = "YOURAPIKEY";
const char *latitude = "YOURLATITUDE";
const char *longitude = "YOURLONGITUDE";

int weatherInnerColor[] = {
  0xfc44, 0xf7a6, 0x8e1e, 0xffde, 0xf79e, 0xffff,
  0xffff, 0x6bf1, 0x6bf1, 0x6bf1, 0x0000, 0x19ad,
};

int weatherOuterColor[] = {
  0xf800, 0xe7ea, 0x19ad, 0xe77e, 0xe77e, 0xe77e,
  0xe77e, 0x39e9, 0x39e9, 0x39e9, 0x1111, 0x8e1e,
};

struct {
  int x;
  int y;
} coordinate[] = {
  {64, 13}, {87, 19}, {103, 36}, {110, 59}, {104, 82}, {87, 99},
  {64, 105}, {41, 99}, {24, 82}, {18, 59}, {24, 36}, {41, 19},
};

void printInfo(void *arg) {
  struct tm timeInfo;
  int needUpdate;
  char s[9];

  while (1) {
    static time_t lastUpdate = 0;
    int colorIndex;

    // needUpdate = (dsParser.lastUpdate == 0) || (lastUpdate <= (dsParser.lastUpdate + 5));
    needUpdate = lastUpdate <= (dsParser.lastUpdate + 5);
    getLocalTime(&timeInfo);
    sprintf(s, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    oled.textForegroundColor(0xffff);
    oled.moveCursor(5, 7);
    oled.putString(s);
    if (needUpdate) {
      oled.moveCursor(6, 5);
      sprintf(s, "%5.1fC", dsParser.weatherData[0].temperature);
      oled.putString(s);
      oled.moveCursor(6, 9);
      sprintf(s, "%5.1f%%", dsParser.weatherData[0].humidity);
      oled.putString(s);
    }
    for (int i = 0, pos = dsParser.currentHour; i < 13; i++) {
      if (i == 1) {
        continue;
      }

      if ((dsParser.weatherData[i].weather == 2) && (dsParser.weatherData[i].precipIntensity > 3)) {
        colorIndex = 11;
      } else {
        colorIndex = dsParser.weatherData[i].weather;
      }

      if (needUpdate) {
        oled.drawFilledCircle(coordinate[pos % 12].x, coordinate[pos % 12].y, 9, weatherInnerColor[colorIndex]);
      }
      if (i == 0) { /* current hour */
        if (timeInfo.tm_sec % 2) {
          oled.drawCircle(coordinate[pos % 12].x, coordinate[pos % 12].y, 9, weatherInnerColor[colorIndex]);
          oled.drawCircle(coordinate[pos % 12].x, coordinate[pos % 12].y, 8, weatherInnerColor[colorIndex]);
        } else {
          oled.drawCircle(coordinate[pos % 12].x, coordinate[pos % 12].y, 9, weatherOuterColor[colorIndex]);
          oled.drawCircle(coordinate[pos % 12].x, coordinate[pos % 12].y, 8, weatherInnerColor[colorIndex]);
        }
      }
      pos++;
    }
    lastUpdate = time(NULL);
    delay(100);
  }
}

void setup() {
  uint16_t result;
  Serial2.begin(9600);

  oled.begin();
  oled.clearScreen();
  oled.mediaInit(&result);
  oled.screenSaverTimeout(0);
  oled.drawCircle(64, 59, 34, 0xc659);
  oled.drawCircle(64, 59, 58, 0xc659);
  oled.moveCursor(0, 15);
  oled.textForegroundColor(0x122e);
  oled.putString("Powered by DarkSky");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  dsParser.begin(apiKey, latitude, longitude);
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  xTaskCreatePinnedToCore(printInfo, "printInfo", 2048, NULL, 1, NULL, 0);
}

void loop() {
  dsParser.getData();
  delay(((300 - (time(NULL) % 300)) + 10) * 1000);
}
