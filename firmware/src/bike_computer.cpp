#include "Arduino.h"
#include "bike_computer.h"

#define THRESHOLD 300
#define VERIFY_ITER_N 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define PI 3.14159265
#define BIKE_RADIUS 0.20 // in m

void BikeComputer::Init() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
    display.Init();
    delay(500);
}

void BikeComputer::HandleLoop() {
    if (UpdateTriggerState()) {
        distance = 2 * PI * BIKE_RADIUS * num_revolutions; // in m
        display.RevsAndDistance(num_revolutions, distance);
    }
}

bool BikeComputer::UpdateTriggerState() {
    // TODO: count and timestamp detected unique revolutions.
    if (IsSwitchTriggered() && !prev_trigger_state) {
        // New unique revolution
        prev_trigger_state = true;
        num_revolutions += 1;
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
