#ifndef FIRMWARE_BIKE_COMPUTER_H
#define FIRMWARE_BIKE_COMPUTER_H
#include "display.h"

class BikeComputer {
public:
    BikeComputer(): display(), prev_trigger_state(false), num_revolutions(0) {};

    void Init();
    void HandleLoop();

private:
    bool IsSwitchTriggered();
    bool UpdateTriggerState();

    BikeDisplay display;
    bool prev_trigger_state;
    int num_revolutions;
    double distance;
};

#endif //FIRMWARE_BIKE_COMPUTER_H
