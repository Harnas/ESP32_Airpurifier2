#pragma once

#include <Adafruit_BME280.h>
#include <DustSensor.hpp>
class Sensors
{
private:
    Adafruit_BME280 *_BME;
    DustSensor *_dustSensor;
    bool BMEStatus;

public:
    float temperature;
    float humidity;
    float pressure;
    float dust;

    // Default Constructor
    Sensors(Adafruit_BME280 *BME, DustSensor *dustSensor);
    void collectBME(void);
    void collectDUST(void);
    void collectSensorsValues(void);
    void printValues(void);
};
