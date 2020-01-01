#pragma once

#include <Arduino.h>
#include <EEPROM.h>

enum Driver_mode
{
    mode_Auto,
    mode_Manual
};

#define max_credencial_lenght 10
#define max_credencials 5

struct WiFi_credencial_struct
{
    char SSID[max_credencial_lenght] = {0};
    char password[max_credencial_lenght] = {0};
};

struct Settings_struct
{
    int8_t manual_speed;
    int8_t auto_speed;
    Driver_mode mode;
    bool internet;
    WiFi_credencial_struct WiFi_credencials[max_credencials];
    uint32_t checksum;
};

class Settings
{
    Settings_struct _tmp_settings;

public:
    Settings_struct settings;

    Settings(void);
    void begin(void);

    void load(void);
    void save(void);

    void default_settings(void);
};