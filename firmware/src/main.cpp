/* 
 * Initial WIP program to sample an exercise bike's
 * magnetic reed switch. 
 * @author Suyash Kumar <suyash@suyashkumar.com>
 */
#include "Arduino.h"
#include "bike_computer.h"

BikeComputer bike_computer;

void setup() {
    bike_computer.Init();
}

void loop() {
    bike_computer.HandleLoop();
}
