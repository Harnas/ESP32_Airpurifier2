#pragma once

#include <Arduino.h>

class Fan
{
    float a, b, V_fan_max, V_fan_min;
    int PWM_max_duty;

    int en_pin, pwm_pin, pwm_channel;

public:
    int duty;
    int power;

public:
    Fan(int enpin, int pwmpin, int pwmchannel);

    int set_pwm(int val);
    int set(int val);
    void max();
    void off();
};