#include <WiFi.h>
#include "goldelox.h"
#include "JsonListener.h"
#include "DarkskyParser.h"

DarkskyParser dlParser;

HardwareSerial Serial2(2);
GOLDELOX oled(&Serial2);

const char *ssid = "YOURSSID";
const char *password = "YOURPASSWORD";

const char *apiKey = "YOURAPIKEY";
const char *latitude = "YOURLATITUDE";
const char *longitude = "YOURLONGITUDE";

int weatherInnerColor[] = {
  0xfc44, 0xf7a6, 0x32b4, 0xffde, 0xf79e, 0xffff,
  0xffff, 0x6bf1, 0x6bf1, 0x6bf1, 0x0000, 0x19ad,
};

int weatherOuterColor[] = {
  0xf800, 0xe7ea, 0x21ab, 0xe77e, 0xe77e, 0xe77e,
  0xe77e, 0x39e9, 0x39e9, 0x39e9, 0x1111, 0x0867,
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
  char s[9];

  while (1) {
    int colorIndex;

    oled.textForegroundColor(0xffff);

    getLocalTime(&timeInfo);
    sprintf(s, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    oled.moveCursor(5, 7);
    oled.putString(s);
    oled.moveCursor(6, 5);
    sprintf(s, "%5.1fC", dlParser.weatherData[0].temperature);
    oled.putString(s);
    oled.moveCursor(6, 9);
    sprintf(s, "%5.1f%%", dlParser.weatherData[0].humidity);
    oled.putString(s);

    for (int i = 0, j = 0; i < 13; i++) {
      if (i == 1) {
        continue;
      }

      if ((dlParser.weatherData[j].weather == 2) && (dlParser.weatherData[j].precipIntensity > 3)) {
        colorIndex = 11;
      } else {
        colorIndex = dlParser.weatherData[j].weather;
      }

      int index = j + dlParser.currentHour;
      if (j == 0) { /* current hour */
        oled.drawFilledCircle(coordinate[index % 12].x, coordinate[index % 12].y,
                              9, weatherInnerColor[colorIndex]);
        if (timeInfo.tm_sec % 2) {
          oled.drawCircle(coordinate[index % 12].x, coordinate[index % 12].y, 8, weatherOuterColor[colorIndex]);
          oled.drawCircle(coordinate[index % 12].x, coordinate[index % 12].y, 9, weatherOuterColor[colorIndex]);
        }
      } else {
        oled.drawFilledCircle(coordinate[index % 12].x, coordinate[index % 12].y,
                              9, weatherInnerColor[colorIndex]);
      }
      j++;
    }

    delay(5);
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

  dlParser.begin(apiKey, latitude, longitude);
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  xTaskCreatePinnedToCore(printInfo, "printInfo", 2048, NULL, 1, NULL, 0);
}

void loop() {
  dlParser.getData();
  delay(((300 - (time(NULL) % 300)) + 10) * 1000);
}
