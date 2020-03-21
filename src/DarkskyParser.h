#ifndef DARKSKY_PARSER_H
#define DARKSKY_PARSER_H

#include <WiFiClientSecure.h>
#include "JsonStreamingParser.h"
#include "JsonListener.h"

#define DS_NUMBER_OF_HOURLY_DATA 13

typedef struct {
      int weather;
      float temperature;
      float humidity;
      float precipIntensity;
      int precipProbability;
      float pressure;
} weatherInfo_t;  

class DarkskyParser: public JsonListener {
  public:
    weatherInfo_t weatherData[DS_NUMBER_OF_HOURLY_DATA + 1];
    int currentHour;
    time_t lastUpdate = 0;

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
      "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
      "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
      "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
      "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
      "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
      "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
      "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
      "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
      "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
      "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
      "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
      "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
      "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
      "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
      "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
      "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
      "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
      "rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
      "-----END CERTIFICATE-----";

    char *apiKey;
    char *latitude;
    char *longitude;

    JsonStreamingParser parser;
    WiFiClientSecure client;

    int counter;
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
