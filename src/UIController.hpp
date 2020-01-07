#pragma once

#include <U8g2lib.h>

#include <AirCleanerController.hpp>
#include <Screen.hpp>

enum InteractionTypes
{
    Button_press,
    Knob_rotate,
    Knob_press,
    Internet,
    _InteractionTypesCount
};
enum InteractionInterfaces
{
    interactionPhysical,
    interactionInternet,
    _InteractionInterfacesCount
};

class UIController
{
    Screen *_screen;
    AirCleanerController *_aircleanercontroller;

    unsigned int timeFromlastAction[_InteractionInterfacesCount] = {0};
    int _XtalFreq;

public:
    UIController(Screen *screen, AirCleanerController *aircleanercontroller);
    void resetInteracionTimer(InteractionInterfaces type);
    void incActionTimer(int miliseconds);
    void screenPowerManagement(void);
    void corePowerManagement(void);
    void knobRotation(int delta);
    void knobPress(void);
    void buttonPress(void);

    void init(void);
    void renderScreen(void);
    void refresh(void);
    void input_event_physical(InteractionTypes type, void *parameter);
};