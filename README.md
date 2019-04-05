# DarkSky_uOLED-128-G2
Display weather information from Dark Sky API on uOLED-128-G2 using ESP-WROOM-32 and Arduino IDE.

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
