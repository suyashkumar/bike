#ifndef FIRMWARE_DISPLAY_H
#define FIRMWARE_DISPLAY_H
#include <Adafruit_SSD1306.h>

class BikeDisplay {
public:
    BikeDisplay();

    void Init();
    void Revolutions(const int& revs);
    void Distance(const float& dist);
    void RevsAndDistance(const int& revs, const double& dist);

private:
    void PrepareDisplayForPrint(bool is_center_cursor=false);
    Adafruit_SSD1306 display;
};

#endif //FIRMWARE_DISPLAY_H
