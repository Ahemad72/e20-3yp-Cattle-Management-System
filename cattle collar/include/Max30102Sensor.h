#include <MAX30105.h>
#ifndef MAX30102_SENSOR_H
#define MAX30102_SENSOR_H

class Max30102Sensor
{
public:
    void setup();
    void measureBeats();
    int getHeartRate();


private:
    static const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
    byte rates[RATE_SIZE];           // Array of heart rates
    byte rateSpot = 0;
    long lastBeat = 0; // Time at which the last beat occurred
    int beatAvg;
    float beatsPerMinute;
    bool isPlacedCorrect;

    MAX30105 particleSensor;
};

#endif