#ifndef DARKSKY_PARSER_H
#define DARKSKY_PARSER_H

#include <WiFiClientSecure.h>
#include "JsonStreamingParser.h"
#include "JsonListener.h"

#define DS_NUMBER_OF_HOURLY_DATA 12

typedef struct {
      int weather;
      float temperature;
      float humidity;
      float precipIntensity;
      float precipProbability;
      float pressure;
} weatherInfo_t;  

class DarkskyParser: public JsonListener {
  public:
    weatherInfo_t weatherData[DS_NUMBER_OF_HOURLY_DATA + 1];
    int currentHour;

    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void endDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void startArray();
    virtual void endArray();
    virtual void startObject();
    virtual void endObject();

    void begin(const char *apiKey, const char *latitude, const char *longitude);
    void reset();
    void getData();

  private:
    const char *server = "api.darksky.net";
    const char *url = "https://api.darksky.net/forecast/";
    const char *options = "?exclude=daily,alerts,flags&units=si";

    const char *rootCA =
      "-----BEGIN CERTIFICATE-----\n" \
      "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
      "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
      "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
      "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
      "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
      "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
      "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
      "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
      "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
      "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
      "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
      "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
      "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
      "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
      "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
      "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
      "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
      "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
      "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
      "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
      "-----END CERTIFICATE-----\n";

    char *apiKey;
    char *latitude;
    char *longitude;

    JsonStreamingParser parser;
    WiFiClientSecure client;

    int counter;
    const int numData = 12;
    enum {
      S0, CURRENTLY, HOURLY, HOURLYDATA,
    } status;
    enum {
      NONE, TIME, ICON, PRECIPINTENSITY, PRECIPPROBABILITY, TEMPERATURE, HUMIDITY, PRESSURE,
    } subStatus;

    const char weatherName[10][20] = {
      "clear-day", "clear-night", "rain", "snow", "sleet", "wind", "fog",
      "cloudy", "partly-cloudy-day", "partly-cloudy-night",
    };

    void skipHeaders();
    int getChar();
    void setSubStatus(String key);
    void setData(String value);
};

#endif /* DARKSKY_PARSER_H */
