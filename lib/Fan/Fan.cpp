
#include <Fan.hpp>

Fan::Fan(int enpin, int pwmpin, int pwmchannel)
{
    a = -95.1140873015873;
    b = 1149.92931547619;

    V_fan_max = 12.1;
    V_fan_min = 5;
    PWM_max_duty = 1023;

    duty = 0;
    power = 0;

    en_pin = enpin;
    pwm_pin = pwmpin;
    pwm_channel = pwmchannel;

    pinMode(en_pin, OUTPUT);
    pinMode(pwm_pin, OUTPUT);

    ledcSetup(pwmchannel, 312500, 10);
    ledcAttachPin(pwm_pin, pwm_channel);
}

int Fan::set_pwm(int val)
{
    duty = val;

    digitalWrite(en_pin, 1);
    ledcWrite(pwm_channel, duty);

    return duty;
}

int Fan::set(int val)
{
    if (val > 100)
    {
        val = 100;
    }

    float voltage = val * V_fan_max / 100;

    if (voltage > V_fan_min)
    {
        int PWM_value = voltage * a + b;
        if (PWM_value > PWM_max_duty)
        {
            PWM_value = PWM_max_duty;
        }

        set_pwm(PWM_value);
    }
    else
    {
        val = 0;
        off();
    }
    power = val;
    return power;
}

void Fan::max()
{
    set_pwm(0);
}

void Fan::off()
{
    duty = PWM_max_duty;

    digitalWrite(en_pin, 0);
    ledcWrite(pwm_channel, duty);
}