# DarkSky_uOLED-128-G2

ESP-WROOM-32を使った[天気予報付き時計](https://garretlab.web.fc2.com/arduino/esp32/make/darksky_oled_graphic_display/index.html)のソースコードです。

Display weather information from Dark Sky API on uOLED-128-G2 using ESP-WROOM-32 and Arduino IDE.
The article can be found at 
https://garretlab.web.fc2.com/arduino/esp32/make/darksky_oled_graphic_display/index.html
in Japanese.

You need to change the code below to suit your environment.

```
const char *ssid = "YOURSSID";
const char *password = "YOURPASSWORD";

const char *apiKey = "YOURAPIKEY";
const char *latitude = "YOURLATITUDE";
const char *longitude = "YOURLONGITUDE";

const char *tz = "YOURTIMEZONE";
const char *ntpServer1 = "NTPSERVER1";
const char *ntpServer2 = "NTPSERVER2";
const char *ntpServer3 = "NTPSERVER3";
```

You need the JsonListener.h which is available from arduino library manager.
