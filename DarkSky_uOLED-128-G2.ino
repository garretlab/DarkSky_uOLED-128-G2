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

const char *tz = "YOURTIMEZONE";
const char *ntpServer1 = "NTPSERVER1";
const char *ntpServer2 = "NTPSERVER2";
const char *ntpServer3 = "NTPSERVER3";

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
    0x00, 0x00, 0x00, 0xc0, 0x04, 0xc8, 0x03, 0xf0,
    0x17, 0xfa, 0x0f, 0xfc, 0x0f, 0xfc, 0x3f, 0xff,
    0x0f, 0xfc, 0x0f, 0xfc, 0x17, 0xfa, 0x03, 0xf0,
    0x04, 0xc8, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
  },
  /* 1: clear-night */
  {
    0x07, 0xe0, 0x0f, 0xf8, 0x10, 0x7c, 0x20, 0x3e,
    0x00, 0x1e, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0x0f, 0x00, 0x1e, 0x20, 0x3e, 0x10, 0x7c,
    0x0f, 0xf8, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00,
  },
  /* 2: cloudy */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x38, 0x0f, 0xfc, 0x1f, 0xfe, 0x3f, 0xff,
    0x3f, 0xff, 0x1f, 0xfe, 0x0f, 0xfc, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 3: rain */
  {
    0x00, 0xc0, 0x00, 0xc0, 0x07, 0xf8, 0x0f, 0xfc,
    0x1f, 0xfe, 0x3f, 0xff, 0x20, 0xc1, 0x00, 0xc0,
    0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x04, 0xc0,
    0x04, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00,
  },
  /* 4: heavy rain */
  {
    0x01, 0xf8, 0x13, 0xe0, 0x0f, 0xc8, 0x0f, 0x84,
    0x1f, 0x22, 0x3f, 0x90, 0x3d, 0xc8, 0x38, 0xe0,
    0x32, 0x70, 0x21, 0x38, 0x28, 0x98, 0x04, 0x0c,
    0x02, 0x48, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
  },
  /* 5: snow */
  {
    0x03, 0xf0, 0x07, 0xf8, 0x0e, 0xdc, 0x0e, 0xdc,
    0x0f, 0xfc, 0x06, 0x18, 0x03, 0xf0, 0x0f, 0xfc,
    0x1f, 0xfe, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff,
    0x1f, 0xfe, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00,
  },
  /* 6: sleet/fog(small cloud) */
  {
    0x07, 0x38, 0x0f, 0xfc, 0x1f, 0xfe, 0x3f, 0xff,
    0x3f, 0xff, 0x1f, 0xfe, 0x0f, 0xfc, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 7: wind */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x90,
    0x00, 0xd0, 0x0c, 0x10, 0x12, 0xe6, 0x1a, 0x09,
    0x02, 0x0d, 0x3c, 0x01, 0x01, 0xfe, 0x00, 0x00,
    0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 8: partly cloudy(cloud part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x80, 0x0f, 0xc0, 0x1f, 0xe0,
    0x3f, 0xf0, 0x3f, 0xf0, 0x1f, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 9: sleet(snow part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x35, 0x80, 0x0e, 0x00, 0x0e, 0x00,
    0x35, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  },
  /* 10: sleet(rain part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x24, 0x00, 0x24, 0x00, 0x12, 0x00, 0x12,
    0x00, 0x09, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00,
  },
  /* 11: fog(fog part) */
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x0f, 0xfc,
    0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00,
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
  {{0, 8, -1}, {0xfc44, 0xf7de, -1}, 0xa596},             /* 9: partly-cloudy-day */
  {{1, 8, -1}, {0xf7a6, 0xf7de, -1}, 0xa596},             /* 10: partly-cloudy-night */
  {{12, -1}, {0xf800, -1}, 0xf800},                       /* 11: unavailable */
  {{13, -1}, {0xffff, -1}, 0x9491},                       /* 12: other */
};

/* coordinates for each hour in the clock */
struct coordinate_t {
  int x;
  int y;
} coordinate[] = {
  {57, 7}, {80, 13}, {96, 30}, {102, 52}, {96, 75}, {80, 91},
  {57, 97}, {35, 91}, {18, 75}, {12, 52}, {18, 30}, {35, 13},
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
  /* delete old icon */
  oled.drawFilledRectangle(coordinate[hour].x, coordinate[hour].y,
                           coordinate[hour].x + 15, coordinate[hour].y + 16, 0x0000);
  int n = 0;
  while (weatherIconMapping[weather].icon[n] != -1) {
    oled.drawPattern(coordinate[hour].x, coordinate[hour].y, 16, 16,
                     weatherIcon[weatherIconMapping[weather].icon[n]],
                     weatherIconMapping[weather].iconColor[n]);
    n++;
  }
}

void drawPrecipProbability(int hour, int precipProbability) {
  /* delete old precipitation probability */
  oled.drawRectangle(coordinate[hour].x - 1 , coordinate[hour].y,
                     coordinate[hour].x + 1, coordinate[hour].y + 12, 0x0000);
  for (int i = 0; i < ((precipProbability + 10) / 20); i ++) {
    oled.drawFilledRectangle(coordinate[hour].x - 1, coordinate[hour].y + 12 - 3 * i,
                             coordinate[hour].x + 1, coordinate[hour].y + 13 - 3 * i,
                             0x2377);
  }
}

/* display routine */
void printInfo(void *arg) {
  int currentWeather[12], lastWeather[12];
  int currentPrecipProbability[12];
  struct tm timeInfo;
  time_t lastUpdate = 0;
  int lastHour = -1;
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
        if (i == 1) { /* skip dsParser.weatherData[1]. it is forecast data of current hour so abandon it. */
          continue;
        }
        /* set weather and precipitation probablity */
        currentWeather[pos % 12] =
          darkskyWeatherToIcon(dsParser.weatherData[i].weather, dsParser.weatherData[i].precipIntensity);
        currentPrecipProbability[pos % 12] = dsParser.weatherData[i].precipProbability;
        pos++;
      }
      lastUpdate = dsParser.lastUpdate;
    }

    /* draw weather icon and precipProbability */
    for (int i = 0; i < 12; i++) {
      if (currentWeather[i] != lastWeather[i]) {
        oled.moveCursor(5, 7);
        oled.putString("updating"); /* it may take a long time so hide current time */
        drawWeatherIcon(i, currentWeather[i]);
        if (currentWeather[i] < UNAVAILABLE) {
          drawPrecipProbability(i, currentPrecipProbability[i]);
        }
        lastWeather[i] = currentWeather[i];
      }
    }

    /* write temperature */
    if (lastTemperature != dsParser.weatherData[0].temperature) {
      sprintf(s, "%5.1fC", dsParser.weatherData[0].temperature);
      oled.moveCursor(6, 5);
      oled.putString(s);
      lastTemperature = dsParser.weatherData[0].temperature;
    }

    getLocalTime(&timeInfo);
    sprintf(s, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    oled.moveCursor(5, 7);
    oled.putString(s);

    /* hour changed. update the last hour data. delete underline at previous hour */
    if (lastHour != timeInfo.tm_hour) {
      int t = (timeInfo.tm_hour + 11) % 12;
      currentWeather[t] =
        darkskyWeatherToIcon(dsParser.weatherData[13].weather, dsParser.weatherData[13].precipIntensity);
      /* delete underline */
      oled.drawFilledRectangle(coordinate[t].x, coordinate[t].y + 14,
                               coordinate[t].x + 15, coordinate[t].y + 15, 0x0000);
      lastHour = timeInfo.tm_hour;
    }
    /* draw underline at current hour */
    struct coordinate_t c = coordinate[timeInfo.tm_hour % 12];
    oled.drawFilledRectangle(c.x, c.y + 14, c.x + 15, c.y + 15,
                             (timeInfo.tm_sec % 2) ? 0x0000 : weatherIconMapping[currentWeather[timeInfo.tm_hour % 12]].underbarColor);

    /* draw humidity */
    if (lastHumidity != dsParser.weatherData[0].humidity) {
      sprintf(s, "%5.1f%%", dsParser.weatherData[0].humidity);
      oled.moveCursor(6, 9);
      oled.putString(s);
      lastHumidity = dsParser.weatherData[0].humidity;
    }

    delay(100);
  }
}

void setup() {
  const int circleColor = 0x29e8;
  Serial.begin(115200);
  Serial2.begin(9600);

  oled.begin();
  oled.clearScreen();

  oled.screenSaverTimeout(0);
  oled.drawCircle(64, 59, 58, circleColor);
  oled.drawCircle(64, 59, 32, circleColor);
  oled.moveCursor(0, 15);
  oled.textForegroundColor(0x122e);
  oled.putString("Powered by DarkSky");

  xTaskCreatePinnedToCore(printInfo, "printInfo", 2048, NULL, 1, NULL, 0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  dsParser.begin(apiKey, latitude, longitude);
  configTzTime(tz, ntpServer1, ntpServer2, ntpServer3);
}

void loop() {
  dsParser.getData();
  Serial.printf("hour = %d\n", dsParser.currentHour);
  for (int i = 0; i < 14; i++) {
    Serial.printf("%02d: w = %2d, t = %4.1fC, h = %4.1f%%, p = %4d%%, r = %4.1fmm\n",
                  i,
                  dsParser.weatherData[i].weather,
                  dsParser.weatherData[i].temperature,
                  dsParser.weatherData[i].humidity,
                  dsParser.weatherData[i].precipProbability,
                  dsParser.weatherData[i].precipIntensity);
  }
  Serial.printf("Free Heap = %d\n", ESP.getFreeHeap());
  delay(((300 - (time(NULL) % 300)) + 10) * 1000);
}
