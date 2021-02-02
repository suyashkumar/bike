#ifndef FIRMWARE_BIKE_SERVER_CLIENT_H
#define FIRMWARE_BIKE_SERVER_CLIENT_H

#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"
#include <ESP8266HTTPClient.h>

template <typename Payload>
struct StatusOr {
    // HTTP status code
    int status_code;
    Payload payload;
};

class BikeServerClient {
public:
    BikeServerClient() {};
    StatusOr<String> NewSession();
    int AddBatch(const String& session_id, int num_revolutions, unsigned long start_time, unsigned long end_time);

private:
    WiFiClient wifi_client;
    HTTPClient http_client;
};

#endif //FIRMWARE_BIKE_SERVER_CLIENT_H
