/* 
 * Initial WIP program to sample an excercise bike's 
 * magnetic reed switch. 
 * @author Suyash Kumar <suyash@suyashkumar.com>
 */
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif 

#define THRESHOLD 300 
#define VERIFY_ITER_N 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
bool previous_trigger_state = false;
int num_revolutions = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Start.");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16 - 4);      
  display.println(F("Bike!"));
  display.display();
  delay(2000);

}

bool switch_is_triggered() {
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

void loop() {
  // TODO: count and timestamp detected unique revolutions.
  if (switch_is_triggered() && !previous_trigger_state) { 
    // New unique revolution
    previous_trigger_state = true;
	num_revolutions += 1;
  	display.clearDisplay();
  	display.setTextSize(2);             
  	display.setTextColor(SSD1306_WHITE);
  	display.setCursor(0, 16 - 4);      
  	display.print(F("Revs: "));
  	display.print(num_revolutions);
  	display.display();

    // Turn the LED on if the magnetic reed switch is currently triggered
    digitalWrite(LED_BUILTIN, LOW);
  } else if (!switch_is_triggered() && previous_trigger_state) {
    // Switch is not triggered anymore, update previous trigger state
	previous_trigger_state = false;
    // Turn the LED off if the magnetic reed switch is not triggered
    digitalWrite(LED_BUILTIN, HIGH);
  } 

  delay(10);
}
