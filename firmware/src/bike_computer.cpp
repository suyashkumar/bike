#include "Arduino.h"
#include "bike_computer.h"
#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"

#define THRESHOLD 300
#define VERIFY_ITER_N 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define PI 3.14159265
#define BIKE_RADIUS 0.20 // in m
#define BUFFER_UPLOAD_PERIOD 1000 // in ms

// These variables are set at build time using platformio,
// specifically using the $SSID and $WIFIPASSPHRASE
// environment variables.
#ifndef WIFI_SSID
#define WIFI_SSID "test"
#endif
#ifndef WIFI_PASSPHRASE
#define WIFI_PASSPHRASE "test"
#endif

void BikeComputer::Init() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
    display.Init();
    delay(100);

    InitWiFi();
    session_id = bike_server_client.NewSession().payload;
    Serial.println(session_id);

    relative_start_time = millis();
    buffer_relative_start_time = millis();
}

void BikeComputer::HandleLoop() {
    if (UpdateTriggerState()) {
        distance = 2 * PI * BIKE_RADIUS * num_revolutions; // in m
        display.RevsAndDistance(num_revolutions, distance);
    }

    if ((millis() - buffer_relative_start_time) >= BUFFER_UPLOAD_PERIOD) {
        // Trigger upload of this batch
        Serial.println("Upload");
        bike_server_client.AddBatch(session_id, num_revolutions_buffer, buffer_relative_start_time, millis());
        buffer_relative_start_time = millis();
        num_revolutions_buffer = 0;
    }

    delay(10);
}

void BikeComputer::InitWiFi() {
    WiFi.mode(WIFI_STA);
    wifi_multi.addAP(WIFI_SSID, WIFI_PASSPHRASE);

    while (wifi_multi.run() != WL_CONNECTED) {
        Serial.println("connecting...");
        delay(200);
    }
}

bool BikeComputer::UpdateTriggerState() {
    // TODO: count and timestamp detected unique revolutions.
    if (IsSwitchTriggered() && !prev_trigger_state) {
        // New unique revolution
        prev_trigger_state = true;
        num_revolutions += 1;
        num_revolutions_buffer += 1;
        return true;

    } else if (!IsSwitchTriggered() && prev_trigger_state) {
        // Switch is not triggered anymore, update previous trigger state
        prev_trigger_state = false;
    }

    delay(10);
    return false;
}

bool BikeComputer::IsSwitchTriggered() {
        // TODO: can probably use digitalRead instead of analogRead and threshold
        if (analogRead(A0) < THRESHOLD) {
            // Do a couple more reads to verify that we should really count this as a trigger
            int summed_values = 0;
            for (int i = 0; i < VERIFY_ITER_N; i++) {
                summed_values += analogRead(A0);
                delay(4);
            }
            if (summed_values < (THRESHOLD * VERIFY_ITER_N)) {
                return true;
            }
        }

        return false;
}
