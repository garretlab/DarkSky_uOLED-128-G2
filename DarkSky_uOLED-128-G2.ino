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

/* weather definition */
enum {
  CLEAR_DAY = 0, CLEAR_NIGHT = 1, CLOUDY = 2,
  RAIN = 3, HEAVY_RAIN = 4,
  SNOW = 5, SLEET = 6, WIND = 7, FOG = 8,
  PARTLY_CLOUDY_DAY = 9, PARTLY_CLOUDY_NIGHT = 10,
  UNAVAILABLE = 11, OTHER = 12, INITIAL = 13,
};

/* icon difinition for each weather */
uint8_t weatherIcon[][32] = {
  /* 0: clear-day */
  {
    0x01, 0x80, 0x01, 0x80, 0x09, 0x90, 0x47, 0xe2,
    0x2f, 0xf4, 0x1f, 0xf8, 0x1f, 0xf8, 0xff, 0xff,
    0x1f, 0xf8, 0x1f, 0xf8, 0x2f, 0xf4, 0x47, 0xe2,
    0x09, 0x90, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00,
  },
  /* 1: clear-night */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x1f, 0xf8, 0x20, 0xfc,
    0x00, 0x7e, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x3f,
    0x00, 0x3f, 0x00, 0x3e, 0x00, 0x7e, 0x20, 0xfc,
    0x1f, 0xf8, 0x0f, 0xf0, 0x03, 0xc0, 0x00, 0x00,
  },
  /* 2: cloudy */
  {
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1f, 0x80,
    0x1f, 0xcc, 0x3f, 0xfe, 0x3f, 0xfe, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe,
    0x7f, 0xfc, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00,
  },
  /* 3: rain */
  {
    0x01, 0x80, 0x01, 0x80, 0x0f, 0xf0, 0x3f, 0xfc,
    0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x81, 0x83,
    0x81, 0x81, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
    0x11, 0x80, 0x09, 0x80, 0x07, 0x00, 0x00, 0x00,
  },
  /* 4: heavy rain */
  {
    0x00, 0x78, 0x47, 0xfc, 0x2f, 0xc0, 0x1f, 0x88,
    0x3f, 0x24, 0x7e, 0x10, 0x7f, 0x08, 0x7b, 0x80,
    0x71, 0xc0, 0xe0, 0xe0, 0xc8, 0x70, 0xc4, 0x38,
    0x52, 0x1c, 0x08, 0x8c, 0x00, 0x78, 0x00, 0x00,
  },
  /* 5: snow */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x19, 0x98, 0x19, 0x98,
    0x1e, 0x78, 0x1e, 0x78, 0x0f, 0xf0, 0x3c, 0x3c,
    0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe,
    0x7f, 0xfe, 0x3f, 0xfc, 0x0f, 0xf0, 0x00, 0x00,
  },
  /* 6: sleet/fog(small cloud) */
  {
    0x06, 0x60, 0x0f, 0xf0, 0x3f, 0xfc, 0x7f, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 7: wind */
  {
    0x00, 0x00, 0x00, 0x60, 0x00, 0x90, 0x00, 0xd0,
    0x00, 0x10, 0x19, 0xe0, 0x24, 0x0c, 0x32, 0x12,
    0x04, 0x1a, 0x78, 0x02, 0x03, 0xfc, 0x00, 0x00,
    0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 8: partly cloudy(cloud part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x07, 0x80, 0x1f, 0xe0,
    0x3f, 0xf0, 0x7f, 0xf8, 0x7f, 0xf8, 0x3f, 0xf8,
    0x1f, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 9: sleet(snow part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x6b, 0x00, 0x1c, 0x00,
    0x1c, 0x00, 0x6b, 0x00, 0x08, 0x00, 0x00, 0x00,
  },
  /* 10: sleet(rain part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x48, 0x00, 0x48, 0x00, 0x24,
    0x00, 0x24, 0x00, 0x12, 0x00, 0x12, 0x00, 0x00,
  },
  /* 11: fog(fog part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00,
    0x1f, 0xf8, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00,
  },
  /* 12: unavailable */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x18, 0x18, 0x31, 0x8c,
    0x61, 0x86, 0x41, 0x82, 0xc1, 0x83, 0xc1, 0x83,
    0xc1, 0x83, 0x40, 0x02, 0x61, 0x86, 0x31, 0x8c,
    0x18, 0x18, 0x0f, 0xf0, 0x03, 0xc0, 0x00, 0x00,
  },
  /* 13: other */
  {
    0x03, 0xc0, 0x0f, 0xf0, 0x18, 0x18, 0x33, 0xcc,
    0x66, 0x66, 0x4c, 0x32, 0xc4, 0x23, 0xc0, 0x43,
    0xc1, 0x83, 0x41, 0x82, 0x60, 0x06, 0x31, 0x8c,
    0x18, 0x18, 0x0f, 0xf0, 0x03, 0xc0, 0x00, 0x00,
  },
};

/* weather and icon mapping. displayed by the order and should be end with -1 */
struct weatherIconMapping_t {
  int icon[4];
  int iconColor[4];
  int underbarColor;
} weatherIconMapping[] = {
  {{0, -1}, {0xfc44, -1}, 0xf800},                        /* 0: clear-day */
  {{1, -1}, {0xf7a6, -1}, 0xe585},                        /* 1: clear-night */
  {{2, -1}, {0xce79, -1}, 0x9491},                        /* 2: cloudy */
  {{3, -1}, {0x2377, -1}, 0x126d},                        /* 3: rain */
  {{4, -1}, {0x19ad, -1}, 0x126d},                        /* 4: heavy rain */
  {{5, -1}, {0xffff, -1}, 0xdefb},                        /* 5: snow */
  {{6, 9, 10, -1}, {0xce79, 0xffff, 0x2377, -1}, 0xa596}, /* 6: sleet */
  {{7, -1}, {0xce79, -1}, 0xa596},                        /* 7: wind */
  {{6, 11, -1}, {0xce79, 0xce79, -1}, 0xa596},            /* 8: fog */
  {{0, 8, -1}, {0xfc44, 0xce79, -1}, 0xa596},             /* 9: partly-cloudy-day */
  {{1, 8, -1}, {0xf7a6, 0xce79, -1}, 0xa596},             /* 10: partly-cloudy-night */
  {{12, -1}, {0xf800, -1}, 0xf800},                       /* 11: unavailable */
  {{13, -1}, {0xffff, -1}, 0x9491},                       /* 12: other */
};

/* coordinates for each hour in the clock */
struct coordinate_t {
  int x;
  int y;
} coordinate[] = {
  {64, 13}, {86, 19}, {103, 36}, {110, 59}, {103, 82}, {86, 99},
  {64, 105}, {41, 99}, {24, 82}, {18, 59}, {24, 36}, {41, 19},
};

uint8_t dataIcon[][8] {
  {0x18, 0x00, 0x18, 0x18, 0x18, 0x3c, 0x7e, 0x3c},
  {0x08, 0x08, 0x1c, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c},
  {0x18, 0x3c, 0x7e, 0xff, 0x18, 0x18, 0x58, 0x30},
};

/* mapping from darksky weather to this clock weather */
/* rain is split into usual rain and heavy rain */
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
    case 4: /* sleet */
      return SLEET;
      break;
    case 5:
      return WIND;
      break;
    case 6:
      return FOG;
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

/* draw weather icon at specified hour position */
void drawWeatherIcon(int hour, int weather) {
  /* clear old icon */
  oled.drawFilledRectangle(coordinate[hour].x - 7, coordinate[hour].y - 8,
                           coordinate[hour].x + 9, coordinate[hour].y + 8, 0x0000);
  int n = 0;
  while (weatherIconMapping[weather].icon[n] != -1) {
    oled.drawPattern(coordinate[hour].x - 7, coordinate[hour].y - 8, 16, 16,
                     weatherIcon[weatherIconMapping[weather].icon[n]],
                     weatherIconMapping[weather].iconColor[n]);
    n++;
  }
}

/* display routine */
void printInfo(void *arg) {
  int currentWeather[12], lastWeather[12];
  struct tm timeInfo;
  time_t lastUpdate = 0;
  int lastHour = -1, lastPrecipProbability = -1, lastDisplay = -1;
  float lastTemperature = 0, lastHumidity = 0;
  char s[9];

  oled.textForegroundColor(0xffff);

  while (1) {
    time_t currentTime = time(NULL);

    /* check if the data is valid */
    if ((dsParser.lastUpdate == 0) || ((currentTime - dsParser.lastUpdate) > 3600)) {
      /* no data or data is too old */
      for (int i = 0; i < 12; i++) {
        currentWeather[i] = UNAVAILABLE;
      }
    } else if (lastUpdate < dsParser.lastUpdate) {
      /* dark sky data has been updated */
      for (int i = 0, pos = dsParser.currentHour; i < 13; i++) {
        if (i == 1) { /* skip dsParser.weatherData[1]. it is forecast data of current hour */
          continue;
        }
        currentWeather[pos % 12] =
          darkskyWeatherToIcon(dsParser.weatherData[i].weather, dsParser.weatherData[i].precipIntensity);
        pos++;
      }
      lastUpdate = dsParser.lastUpdate;
    }

    /* draw weather icon */
    for (int i = 0; i < 12; i++) {
      if (currentWeather[i] != lastWeather[i]) {
        oled.moveCursor(5, 7);
        oled.putString("updating");
        drawWeatherIcon(i, currentWeather[i]);
        lastWeather[i] = currentWeather[i];
      }
    }

    /* draw temperature */
    if (lastTemperature != dsParser.weatherData[0].temperature) {
      oled.moveCursor(7, 5);
      sprintf(s, "%5.1fC", dsParser.weatherData[0].temperature);
      oled.putString(s);
      oled.drawPattern(40, 40, 8, 8, dataIcon[0], 0xf800);
      lastTemperature = dsParser.weatherData[0].temperature;
    }

    getLocalTime(&timeInfo);
    sprintf(s, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    oled.moveCursor(5, 7);
    oled.putString(s);

    /* hour changed. update the last hour data. delete underline at previous hour */
    if (lastHour != timeInfo.tm_hour) {
      int t = (timeInfo.tm_hour - 1) % 12;
      currentWeather[t] =
        darkskyWeatherToIcon(dsParser.weatherData[13].weather, dsParser.weatherData[13].precipIntensity);
      oled.drawFilledRectangle(coordinate[t].x - 7, coordinate[t].y + 6,
                               coordinate[t].x + 7, coordinate[t].y + 7, 0x0000);
      lastHour = timeInfo.tm_hour;
    }
    /* draw underline at current hour */
    struct coordinate_t c = coordinate[timeInfo.tm_hour % 12];
    oled.drawFilledRectangle(c.x - 7, c.y + 7, c.x + 7, c.y + 8,
                             (timeInfo.tm_sec % 2) ? 0x0000 : weatherIconMapping[currentWeather[timeInfo.tm_hour % 12]].underbarColor);

    if ((timeInfo.tm_sec / 30) % 2) {
      /* draw humidity */
      if ((lastDisplay != 0) || (lastHumidity != dsParser.weatherData[0].humidity)) {
        oled.moveCursor(7, 9);
        sprintf(s, "%5.1f%%", dsParser.weatherData[0].humidity);
        oled.putString(s);
        oled.drawFilledRectangle(40, 72, 48, 80, 0x0000);
        oled.drawPattern(40, 72, 8, 8, dataIcon[1], 0x2377);
        lastHumidity = dsParser.weatherData[0].humidity;
        lastDisplay = 0;
      }
    } else {
      /* draw precipProbability */
      if ((lastDisplay != 1) || (lastPrecipProbability != dsParser.weatherData[0].precipProbability)) {
        oled.moveCursor(7, 9);
        sprintf(s, "%5d%%", dsParser.weatherData[0].precipProbability);
        oled.putString(s);
        oled.drawFilledRectangle(40, 72, 48, 80, 0x0000);
        oled.drawPattern(40, 72, 8, 8, dataIcon[2], 0x2377);
        lastPrecipProbability = dsParser.weatherData[0].precipProbability;
        lastDisplay = 1;
      }
    }
    delay(100);
  }
}

void setup() {
  uint16_t result;
  const int circleColor = 0x29e8;
  Serial2.begin(9600);

  oled.begin();
  oled.clearScreen();

  oled.screenSaverTimeout(0);
  oled.drawCircle(64, 59, 58, circleColor);
  oled.drawCircle(64, 59, 34, circleColor);
  oled.moveCursor(0, 15);
  oled.textForegroundColor(0x122e);
  oled.putString("Powered by DarkSky");

  xTaskCreatePinnedToCore(printInfo, "printInfo", 2048, NULL, 1, NULL, 0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  dsParser.begin(apiKey, latitude, longitude);
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
}

void loop() {
  dsParser.getData();
  delay(((300 - (time(NULL) % 300)) + 10) * 1000);
}
