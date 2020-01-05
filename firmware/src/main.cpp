/* 
 * Initial WIP program to sample an excercise bike's 
 * magnetic reed switch. 
 * @author Suyash Kumar <suyash@suyashkumar.com>
 */
#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif 

#define THRESHOLD 300 
#define VERIFY_ITER_N 10

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Start.");
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
  if (switch_is_triggered()) { 
    // Turn the LED on if the magnetic reed switch is currently triggered
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(1); 
  } else {
    // Turn the LED off if the magnetic reed switch is not triggered
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(10);
}
