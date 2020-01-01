#include <UIController.hpp>

#define ACTION_TIMEOUT 20 * 1000

UIController::UIController(Screen *screen, AirCleanerController *aircleanercontroller)
{
    _screen = screen;
    _aircleanercontroller = aircleanercontroller;
}

void UIController::resetInteracionTimer(InteractionTypes type)
{
    timeFromlastAction[type] = 0;
}

void UIController::incActionTimer(int miliseconds)
{
    for (int i = 0; i < _InteractionTypesCount; i++)
    {
        timeFromlastAction[i] += miliseconds;
    }

    screenPowerManagement();
    corePowerManagement();
}

void UIController::screenPowerManagement(void)
{
    if (timeFromlastAction[Button] > ACTION_TIMEOUT && timeFromlastAction[Knob] > ACTION_TIMEOUT)
    {
        _screen->on = false;
        refresh();
    }
    else
    {
        _screen->on = true;
    }
}
void UIController::corePowerManagement(void)
{
    for (int i = 0; i < _InteractionTypesCount; i++)
    {
        if (timeFromlastAction[i] < ACTION_TIMEOUT)
        {
            if (getCpuFrequencyMhz() != 240) // change clock once
            {
                setCpuFrequencyMhz(240); //Set CPU clock to 240MHz
            }
            return;
        }
    }

    if (getCpuFrequencyMhz() != 80) // change clock once
    {
        setCpuFrequencyMhz(80); //Set CPU clock to 80MHz
        _aircleanercontroller->_settings->save();
    }
}

void UIController::knobRotation(int delta)
{
    resetInteracionTimer(Knob);

    Serial.print(_aircleanercontroller->get_fan_power());
    _aircleanercontroller->set_fan_power(_aircleanercontroller->get_fan_power() + delta);
}

void UIController::init(void)
{
    _screen->init();
}
void UIController::renderScreen(void)
{
    Purifer_status_struct *info;
    info = _aircleanercontroller->get_whole_status();

    if (_screen->on == false)
    {
        return;
    }

    if (timeFromlastAction[Knob] < 2000)
    {
        _screen->render_settings_screen(info);
    }
    else
    {
        _screen->render_basic_info_screen(info);
    }
    refresh();
}
void UIController::refresh(void)
{
    _screen->refresh();
}