#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

#include <config.h>

#include <DustSensor.hpp>
#include <Sensors.hpp>

Sensors::Sensors(Adafruit_BME280 *BME, DustSensor *dustSensor)
{
    _BME = BME;
    _dustSensor = dustSensor;
}

void Sensors::collectBME(void)
{
    temperature = _BME->readTemperature();
    pressure = _BME->readPressure();
    Serial.println(temperature);
    // humidity = _BME->readHumidity();
}

void Sensors::collectDUST(void)
{
    _dustSensor->measure_average(20);
    dust = _dustSensor->get_average();
}

void Sensors::collectSensorsValues(void)
{
    collectBME();
    collectDUST();
}

void Sensors::printValues(void)
{
}