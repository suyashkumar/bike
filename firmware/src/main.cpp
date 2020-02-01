/* 
 * Initial WIP program to sample an exercise bike's
 * magnetic reed switch. 
 * @author Suyash Kumar <suyash@suyashkumar.com>
 */
#include "Arduino.h"
#include "display.h"

#define THRESHOLD 300 
#define VERIFY_ITER_N 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

bool previous_trigger_state = false;
int num_revolutions = 0;
BikeDisplay display;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  display.Init();
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
	display.UpdateRevs(num_revolutions);

  } else if (!switch_is_triggered() && previous_trigger_state) {
    // Switch is not triggered anymore, update previous trigger state
	previous_trigger_state = false;
  }

  delay(10);
}
