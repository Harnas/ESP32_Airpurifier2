#include <Arduino.h>
#include <config.h>

#include <DustSensor.hpp>

DustSensor::DustSensor(int ledpin, int adcpin, int refpin)
{
    led_pin = ledpin;
    adc_pin = adcpin;
    ref_pin = refpin;

    buff_fill = 0;

    pinMode(led_pin, OUTPUT_OPEN_DRAIN);

    pinMode(adc_pin, ANALOG);
    pinMode(ref_pin, ANALOG);

    digitalWrite(led_pin, HIGH);
}
void IRAM_ATTR DustSensor::delayMicroseconds(uint32_t us)
{
    uint32_t m = micros();
    if (us)
    {
        uint32_t e = (m + us);
        if (m > e)
        { //overflow
            while (micros() > e)
            {
                NOP();
            }
        }
        while (micros() < e)
        {
            NOP();
        }
    }
}
int DustSensor::read_nowait(void)
{
    digitalWrite(led_pin, LOW);
    delayMicroseconds(280);
    int raw = analogRead(adc_pin);
    digitalWrite(led_pin, HIGH);

    int Vref = analogRead(ref_pin);
    raw = raw * 500 / Vref;

    if (getCpuFrequencyMhz() == 80)
    {
        raw = raw * 160 / 240;
    }

    return raw;
}
int DustSensor::read(void)
{
    int raw = read_nowait();
    delay(11);
    return raw;
}

int DustSensor::compute_formula(int adc_val)
{
    float voltage = adc_val * (3.3 / 1024.0);
    dust = 200 * (voltage - 0.6);
    return dust;
}

int DustSensor::get_average(void)
{
    int i, sum = 0;
    for (i = 0; i < buff_fill; i++)
    {
        sum = sum + values[i];
    }
    int raw = sum / buff_fill;

    return compute_formula(raw);
}
int DustSensor::measure_avg(int num_samples)
{
    read();
    int adc_total = 0, raw = 0;
    for (int i = 0; i < num_samples; ++i)
    {
        raw = read();
        adc_total = adc_total + raw;
    }
    return adc_total / num_samples;
}
void DustSensor::measure_average(int num_samples)
{
    values[buff_fill] = measure_avg(num_samples);
    buff_i = (buff_i + 1) % dust_sensor_buff_size;

    if (buff_fill < dust_sensor_buff_size)
    {
        buff_fill = buff_fill + 1;
    }
    else
    {
        buff_fill = dust_sensor_buff_size;
    }
}
void DustSensor::measure_average_quick(void)
{
    values[buff_fill] = read_nowait();
    buff_i = (buff_i + 1) % dust_sensor_buff_size;

    if (buff_fill < dust_sensor_buff_size)
    {
        buff_fill = buff_fill + 1;
    }
    else
    {
        buff_fill = dust_sensor_buff_size;
    }
}