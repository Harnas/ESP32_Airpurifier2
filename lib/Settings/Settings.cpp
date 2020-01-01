#include <Arduino.h>
#include <EEPROM.h>

#include <Settings.hpp>

#define EEPROM_SIZE 128
#define EEPROM_ADDRESS 0

//TODO settings checksum

Settings::Settings(void)
{
}

void Settings::begin(void)
{
    EEPROM.begin(EEPROM_SIZE);
}
void Settings::load(void)
{
    EEPROM.readBytes(EEPROM_ADDRESS, &_tmp_settings, sizeof(settings));
    memcpy(&settings, &_tmp_settings, sizeof(settings));

    if (settings.checksum != 0xAAAA)
    {
        default_settings();
    }
}
void Settings::save(void)
{
    if (memcmp(&settings, &_tmp_settings, sizeof(settings)) != 0)
    {
        EEPROM.writeBytes(EEPROM_ADDRESS, &settings, sizeof(settings));
        EEPROM.commit();
    }
}

void Settings::default_settings(void)
{
    Serial.println("Settings set to default");
    settings.auto_speed = 70;
    settings.manual_speed = 70;
    settings.internet = true;
    settings.mode = mode_Auto;

    settings.checksum = 0xAAAA;

    EEPROM.writeBytes(EEPROM_ADDRESS, &settings, sizeof(settings));
    EEPROM.commit();
}