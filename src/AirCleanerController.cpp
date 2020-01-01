

#include "AiEsp32RotaryEncoder.h"
#include <config.h>

#include <AirCleanerController.hpp>
#include <Fan.hpp>
#include <Sensors.hpp>
#include <Settings.hpp>

AirCleanerController::AirCleanerController(Settings *settings_controller, Sensors *sensors, Fan *fan, AiEsp32RotaryEncoder *rotaryEncoder)
{
    _settings = settings_controller;
    _sensors = sensors;
    _fan = fan;
    _rotaryEncoder = rotaryEncoder;

    _settings->load();

    last_senor_read = 0;
}

void AirCleanerController::save_settings(void)
{
    _settings->save();
}

void AirCleanerController::read_sensors(void)
{
    last_senor_read = millis();
    _sensors->collectSensorsValues();
}

int AirCleanerController::update_fan_speed(void)
{
    if (_settings->settings.mode == mode_Manual)
    {
        fan_speed = _settings->settings.manual_speed;
    }
    else
    {
        fan_speed = _settings->settings.auto_speed * _sensors->dust / 20;
    }

    fan_speed = _fan->set(fan_speed);
    return fan_speed;
}

int AirCleanerController::get_fan_power(void)
{
    if (_settings->settings.mode == mode_Manual)
    {
        return _settings->settings.manual_speed;
    }
    return _settings->settings.auto_speed;
}

void AirCleanerController::set_fan_power(int8_t val)
{
    set_fan_power(_settings->settings.mode, val);
}

void AirCleanerController::set_fan_power(Driver_mode mode, int8_t val)
{
    val = max((int8_t)0, min((int8_t)100, val));

    if (_settings->settings.mode == mode_Manual)
    {
        _settings->settings.manual_speed = val;
    }
    else
    {
        _settings->settings.auto_speed = val;
    }
}

Purifer_status_struct *AirCleanerController::get_whole_status(void)
{
    _status.dust = _sensors->dust;
    _status.set_power = get_fan_power();
    _status.fan_speed = update_fan_speed();
    _status.mode = _settings->settings.mode;
    _status.temperature = _sensors->temperature;
    _status.pressure = _sensors->pressure;
    _status.humidity = _sensors->humidity;
    _status.internet = false;

    return &_status;
}
