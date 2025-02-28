#include "TempSensor.h"

#define ONE_WIRE_BUS 4

TempSensor::TempSensor() : oneWire(ONE_WIRE_BUS), sensors(&oneWire)
{
    // Initiallize the sensor
}

void TempSensor::setup()
{
    sensors.begin();

    if (!sensors.getAddress(insideThermometer, 0))
        Serial.println("Unable to find address for Device 0");

    // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(insideThermometer, 9);
}

float TempSensor::getTemperature()
{   
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempC(insideThermometer);
    if (tempC == DEVICE_DISCONNECTED_C)
    {
        Serial.println("Error: Could not read temperature data");
        return -1.0f;
    }
    return tempC;
}