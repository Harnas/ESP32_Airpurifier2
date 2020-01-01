#pragma once

#include <Arduino.h>

#include "AiEsp32RotaryEncoder.h"

#include <config.h>

#include <Fan.hpp>
#include <Sensors.hpp>
#include <Settings.hpp>

struct Purifer_status_struct
{
    int dust;
    int fan_speed;
    int set_power;
    Driver_mode mode;
    float temperature;
    float pressure;
    float humidity;
    uint32_t local_adress;
    bool internet;
};

class AirCleanerController
{

    unsigned int last_senor_read;
    Purifer_status_struct _status;

    Fan *_fan;

public:
    Settings *_settings;
    Sensors *_sensors;
    AiEsp32RotaryEncoder *_rotaryEncoder;
    unsigned int fan_speed;

public:
    AirCleanerController(Settings *settings_controller, Sensors *sensors, Fan *fan, AiEsp32RotaryEncoder *rotaryEncoder);
    void save_settings(void);
    void read_sensors(void);
    int update_fan_speed(void);
    int get_fan_power(void);
    void set_fan_power(int8_t val);
    void set_fan_power(Driver_mode mode, int8_t val);
    Purifer_status_struct *get_whole_status(void);
};
