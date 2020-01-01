#pragma once

#include <U8g2lib.h>

#include <AirCleanerController.hpp>
#include <Screen.hpp>

enum InteractionTypes
{
    Button,
    Knob,
    Internet,
    _InteractionTypesCount
};

class UIController
{
    Screen *_screen;
    AirCleanerController *_aircleanercontroller;

    unsigned int timeFromlastAction[_InteractionTypesCount] = {0};

public:
    UIController(Screen *screen, AirCleanerController *aircleanercontroller);
    void resetInteracionTimer(InteractionTypes type);
    void incActionTimer(int miliseconds);
    void screenPowerManagement(void);
    void corePowerManagement(void);
    void knobRotation(int delta);

    void init(void);
    void renderScreen(void);
    void refresh(void);
};