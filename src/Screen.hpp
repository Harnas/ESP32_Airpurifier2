#pragma once

#include <U8g2lib.h>

#include <AirCleanerController.hpp>

class Screen
{
    U8G2 *_screen;
    int brightness;

public:
    bool on;

public:
    Screen(U8G2 *screen);
    void init(void);
    void render_basic_info_screen(Purifer_status_struct *info);
    void render_settings_screen(Purifer_status_struct *info);
    void refresh(void);
};