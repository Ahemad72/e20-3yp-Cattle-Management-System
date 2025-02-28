#include <OneWire.h>
#include <DallasTemperature.h>
#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

class TempSensor
{
public:
    TempSensor();
    void setup();
    float getTemperature();

private:
    OneWire oneWire;
    DallasTemperature sensors;
    DeviceAddress insideThermometer;
};

#endif