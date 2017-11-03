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

enum {
  CLEAR_DAY = 0, CLEAR_NIGHT = 1, CLOUDY = 2, RAIN = 3, HEAVY_RAIN = 4,
  SNOW = 5, PARTLY_CLOUDY = 6, PARTLY_CLOUDY_DAY = 7, PARTLY_CLOUDY_NIGHT = 8, OTHER = 9,
} ;

int weatherIconColor[] = {
  0xfc44, 0xf7a6, 0xce79, 0x2377, 0x19ad, 0xffff, 0xce79, 0xce79, 0xce79, 0xffff,
};

int weatherCurrentColor[] = {
  0xf800, 0xf2e8, 0x9491, 0x126d, 0x126d, 0xdefb, 0xa596, 0xa596, 0xa596, 0xdefb,
};

uint8_t weatherIcon[][32] = {
  /* 0: clear-day */
  {
    0x01, 0x80, 0x01, 0x80, 0x09, 0x90, 0x47, 0xe2,
    0x2f, 0xf4, 0x1f, 0xf8, 0x1f, 0xf8, 0xff, 0xff,
    0xff, 0xff, 0x1f, 0xf8, 0x1f, 0xf8, 0x2f, 0xf4,
    0x47, 0xe2, 0x09, 0x90, 0x01, 0x80, 0x01, 0x80,
  },
  /* 1: clear-night */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x1f, 0xf8, 0x20, 0xfc,
    0x00, 0x7e, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x3f,
    0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3e, 0x00, 0x7e,
    0x20, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x03, 0xc0,
  },
  /* 2: cloudy */
  {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xc0,
    0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfc, 0x7f, 0xfe,
    0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe,
    0x3f, 0xfc, 0x1b, 0xd8, 0x01, 0x80, 0x00, 0x00,
  },
  /* 3: rain */
  {
    0x01, 0x80, 0x01, 0x80, 0x0f, 0xf0, 0x3f, 0xfc,
    0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x87,
    0x81, 0x81, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
    0x01, 0x80, 0x11, 0x80, 0x09, 0x80, 0x07, 0x00,
  },
  /* 4: heavy rain */
  {
    0x00, 0x78, 0x47, 0xfc, 0x2f, 0xc0, 0x1f, 0x88,
    0x3f, 0x24, 0x7e, 0x10, 0x7f, 0x08, 0x78, 0x80,
    0x71, 0xc0, 0xe0, 0xe0, 0xc8, 0x70, 0xc4, 0x38,
    0x52, 0x1c, 0x08, 0x8c, 0x00, 0x48, 0x00, 0x30,
  },
  /* 5: snow */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x19, 0x98, 0x39, 0x9c,
    0x3e, 0x7c, 0x1e, 0x78, 0x0f, 0xf0, 0x1c, 0x38,
    0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x7f, 0xfe, 0x7f, 0xfe, 0x3f, 0xfc, 0x0f, 0xf0,
  },
  /* 6: partly cloudy */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x1f, 0xe0,
    0x3f, 0xf0, 0x7f, 0xf8, 0x7f, 0xf8, 0x7f , 0xf8,
    0x3f, 0xf8, 0x1f, 0xf0, 0x03, 0x00, 0x00, 0x00,
  },
};

struct {
  int x;
  int y;
} coordinate[] = {
  {64, 13}, {87, 19}, {103, 36}, {110, 59}, {104, 82}, {87, 99},
  {64, 105}, {41, 99}, {24, 82}, {18, 59}, {24, 36}, {41, 19},
};

int darkskyWeatherToIcon(int weather, int precipIntensity) {
  switch (weather) {
    case 0: /* clear-day */
      return CLEAR_DAY;
      break;
    case 1: /* clear-night */
      return CLEAR_NIGHT;
      break;
    case 2: /* rain */
      if (precipIntensity < 5) {
        return RAIN;
      } else {
        return HEAVY_RAIN;
      }
      break;
    case 3: /* snow */
      return SNOW;
      break;
    case 7: /* cloudy */
      return CLOUDY;
      break;
    case 8: /* partly-cloudy-day */
      return PARTLY_CLOUDY_DAY;
      break;
    case 9: /* partly-cloudy-night */
      return PARTLY_CLOUDY_NIGHT;
      break;
    default:
      return OTHER;
      break;
  }
}

void drawWeatherIcon(int index, int weather) {
  oled.drawFilledRectangle(coordinate[index].x - 7, coordinate[index].y - 8,
                           coordinate[index].x + 9, coordinate[index].y + 8, 0x0000);

  switch (weather) {
    case CLEAR_DAY: case CLEAR_NIGHT: case CLOUDY: case RAIN: case HEAVY_RAIN: case SNOW:
      oled.drawPattern(coordinate[index].x - 7, coordinate[index].y - 8, 16, 16,
                       weatherIcon[weather], weatherIconColor[weather]);
      break;
    case PARTLY_CLOUDY_DAY:
      oled.drawPattern(coordinate[index].x - 7, coordinate[index].y - 8, 16, 16,
                       weatherIcon[CLEAR_DAY], weatherIconColor[CLEAR_DAY]);
      oled.drawPattern(coordinate[index].x - 7, coordinate[index].y - 8, 16, 16,
                       weatherIcon[PARTLY_CLOUDY], weatherIconColor[PARTLY_CLOUDY]);
      break;
    case PARTLY_CLOUDY_NIGHT:
      oled.drawPattern(coordinate[index].x - 7, coordinate[index].y - 8, 16, 16,
                       weatherIcon[CLEAR_NIGHT], weatherIconColor[CLEAR_NIGHT]);
      oled.drawPattern(coordinate[index].x - 7, coordinate[index].y - 8, 16, 16,
                       weatherIcon[PARTLY_CLOUDY], weatherIconColor[PARTLY_CLOUDY]);
      break;
    default:
      oled.drawFilledCircle(coordinate[index].x, coordinate[index].y, 7, weatherIconColor[OTHER]);
      break;
  }
}

void printInfo(void *arg) {
  int currentWeather[12];
  int prevWeather[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  struct tm timeInfo;
  char s[9];

  while (1) {
    static time_t lastUpdate = 0;
    int needUpdate = 0;

    if ((lastUpdate < (dsParser.lastUpdate + 5))) { /* got new weather information */
      needUpdate = 1;
    } else if ((timeInfo.tm_hour % 12) != dsParser.currentHour) { /* current hour changed */
      needUpdate = 1;
      /* clear underline of previous hour */
      oled.drawFilledRectangle(coordinate[dsParser.currentHour].x - 7, coordinate[dsParser.currentHour].y + 6,
                               coordinate[dsParser.currentHour].x + 7, coordinate[dsParser.currentHour].y + 7, 0x0000);
    } else { /* no need to update weather information */
      needUpdate = 0;
    }
    lastUpdate = time(NULL);

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

      for (int i = 0, pos = dsParser.currentHour; i < 13; i++) {
        if (i == 1) { /* skip dsParser.weatherData[1] */
          continue;
        }

        currentWeather[pos % 12] =
          darkskyWeatherToIcon(dsParser.weatherData[i].weather, dsParser.weatherData[i].precipIntensity);
        if (currentWeather[pos % 12] != prevWeather[pos % 12]) {
          drawWeatherIcon(pos % 12, currentWeather[pos % 12]);
        }
        pos++;
      }

      for (int i = 0; i < 12; i++) {
        prevWeather[i] = currentWeather[i];
      }
    }

    oled.drawFilledRectangle(coordinate[timeInfo.tm_hour % 12].x - 7, coordinate[timeInfo.tm_hour % 12].y + 6,
                             coordinate[timeInfo.tm_hour % 12].x + 7, coordinate[timeInfo.tm_hour % 12].y + 7,
                             (timeInfo.tm_sec % 2) ? weatherCurrentColor[currentWeather[timeInfo.tm_hour % 12]] : 0x0000);
    delay(100);
  }
}

void setup() {
  uint16_t result;
  const int circleColor = 0x29e8;
  Serial2.begin(9600);

  oled.begin();
  oled.clearScreen();
  oled.mediaInit(&result);

  oled.screenSaverTimeout(0);
  oled.drawCircle(64, 59, 58, circleColor);
  oled.drawCircle(64, 59, 34, circleColor);
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
