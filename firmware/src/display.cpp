#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Arduino.h"
#include "display.h"
#include "SPI.h"
#include "Wire.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define LARGE_TEXT_SIZE 2
#define SMALL_TEXT_SIZE 1

BikeDisplay::BikeDisplay() {
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
}

void BikeDisplay::Init() {
    // TODO: make BikeDisplay work with any size display?
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println("SSD1306 allocation failed");
        exit(1);  // TODO: figure out if this error happens often, and if so, how best to handle
    }
    PrepareDisplayForPrint(/*is_center_cursor=*/true);
    display.print(F("Bike!"));
    display.display();
}

void BikeDisplay::UpdateRevs(const int& revs) {
    PrepareDisplayForPrint(/*is_center_cursor=*/true);
    display.print(F("Revs: "));
    display.print(revs);
    display.display();
}

void BikeDisplay::PrepareDisplayForPrint(bool is_center_cursor) {
    display.clearDisplay();
    display.setTextSize(2);
    // TODO: support optional inverted color mode?
    display.setTextColor(SSD1306_WHITE);

    if (is_center_cursor) {
        display.setCursor(0, (SCREEN_HEIGHT / 2) - (4 * LARGE_TEXT_SIZE / 2));
    } else {
        display.setCursor(0, 0);
    }
}




