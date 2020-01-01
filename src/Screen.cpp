
#include <Arduino.h>
#include <U8g2lib.h>

#include <Screen.hpp>

Screen::Screen(U8G2 *screen)
{
    _screen = screen;
    on = true;
    brightness = 20;
}
void Screen::init(void)
{
    _screen->begin();
    _screen->setContrast(brightness);
}
void Screen::render_basic_info_screen(Purifer_status_struct *info)
{
    char buffer[50];

    _screen->clearBuffer();
    _screen->setFont(u8g2_font_ncenR10_tf);
    _screen->setFontPosTop();

    sprintf(buffer, "%dug/m3", info->dust);
    _screen->drawStr(0, 0, buffer);

    sprintf(buffer, "Fan: %d%%", info->fan_speed);
    _screen->drawStr(0, 20, buffer);

    sprintf(buffer, "%.2f\xB0 C", info->temperature);
    _screen->drawStr(0, 40, buffer);
}

void Screen::render_settings_screen(Purifer_status_struct *info)
{
    char buffer[50];

    _screen->clearBuffer();
    _screen->setFont(u8g2_font_ncenR10_tf);
    _screen->setFontPosTop();

    sprintf(buffer, "fan power: %d%%", info->set_power);
    _screen->drawStr(0, 0, buffer);

    if (info->mode == mode_Manual)
    {
        sprintf(buffer, "Mode: manual");
    }
    else
    {
        sprintf(buffer, "Mode: Auto");
    }
    _screen->drawStr(0, 15, buffer);
}
void Screen::refresh(void)
{
    if (on)
    {
        _screen->setContrast(brightness);
        _screen->sendBuffer();
    }
    else
    {
        _screen->setContrast(0);
        _screen->clearBuffer();
        _screen->sendBuffer();
    }
}