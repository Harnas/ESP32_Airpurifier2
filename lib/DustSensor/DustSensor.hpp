#pragma once

#include <Arduino.h>

#define dust_sensor_buff_size 200
class DustSensor
{

    float dust;

public:
    uint16_t values[dust_sensor_buff_size];
    int buff_i, buff_fill;
    int led_pin, adc_pin, ref_pin;
    DustSensor(int ledpin, int adcpin, int refpin);
    void IRAM_ATTR delayMicroseconds(uint32_t us);
    int read_nowait(void);
    int read(void);
    int compute_formula(int adc_val);
    int get_average(void);
    int measure_avg(int num_samples);
    void measure_average(int num_samples);
    void measure_average_quick(void);
};
