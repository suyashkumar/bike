#ifndef FIRMWARE_BIKE_COMPUTER_H
#define FIRMWARE_BIKE_COMPUTER_H
#include "display.h"
#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"
#include <ESP8266HTTPClient.h>
#include "bike_server_client.h"

class BikeComputer {
public:
    BikeComputer(): display(), prev_trigger_state(false), num_revolutions(0), num_revolutions_buffer(0) {};

    void Init();
    void HandleLoop();

private:
    void InitWiFi();
    bool IsSwitchTriggered();
    bool UpdateTriggerState();

    ESP8266WiFiMulti wifi_multi;
    BikeServerClient bike_server_client;

    BikeDisplay display;
    bool prev_trigger_state;
    int num_revolutions;
    double distance;

    String session_id;
    // The relative start time of the session in milliseconds.
    unsigned long relative_start_time;
    unsigned long buffer_relative_start_time;
    // The number of revolutions recorded since we last sent a batch to the server.
    int num_revolutions_buffer;
};

#endif //FIRMWARE_BIKE_COMPUTER_H
