#include <UIController.hpp>

#define ACTION_TIMEOUT 20 * 1000

UIController::UIController(Screen *screen, AirCleanerController *aircleanercontroller)
{
    _screen = screen;
    _aircleanercontroller = aircleanercontroller;
    _XtalFreq = getXtalFrequencyMhz();
}

void UIController::resetInteracionTimer(InteractionInterfaces type)
{
    timeFromlastAction[type] = 0;
}

void UIController::incActionTimer(int miliseconds)
{
    for (int i = 0; i < _InteractionInterfacesCount; i++)
    {
        timeFromlastAction[i] += miliseconds;
    }

    screenPowerManagement();
    corePowerManagement();
}

void UIController::screenPowerManagement(void)
{
    if (timeFromlastAction[interactionPhysical] > ACTION_TIMEOUT)
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
    for (int i = 0; i < _InteractionInterfacesCount; i++)
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

    if (getCpuFrequencyMhz() != _XtalFreq) // change clock once
    {
        setCpuFrequencyMhz(_XtalFreq); //Set CPU clock to Xtal frequency - 24, 26 or 40MHz depends on board
        _aircleanercontroller->_settings->save();
    }
}

void UIController::knobRotation(int delta)
{
    _aircleanercontroller->set_fan_power(_aircleanercontroller->get_fan_power() + delta);
}

void UIController::knobPress(void)
{
    Driver_mode mode = _aircleanercontroller->_settings->settings.mode;

    if (mode == mode_Manual)
    {
        mode = mode_Auto;
    }
    else
    {
        mode = mode_Manual;
    }

    _aircleanercontroller->_settings->settings.mode = mode;
}

void UIController::buttonPress(void)
{
    if (_aircleanercontroller->purifier_on)
    {
        _aircleanercontroller->purifier_on = false;
    }
    else
    {
        _aircleanercontroller->purifier_on = true;
    }
}

void UIController::input_event_physical(InteractionTypes type, void *parameter)
{
    resetInteracionTimer(interactionPhysical);

    if (_screen->on == false)
    {
        _screen->on = true;
        return;
    }

    switch (type)
    {
    case Knob_press:
        knobPress();
        break;
    case Knob_rotate:
        knobRotation((*(int *)parameter));
        break;
    case Button_press:
        buttonPress();
        break;

    default:
        break;
    }

    Driver_mode mode = _aircleanercontroller->_settings->settings.mode;

    if (mode == mode_Manual)
    {
        mode = mode_Auto;
    }
    else
    {
        mode = mode_Manual;
    }

    _aircleanercontroller->_settings->settings.mode = mode;
}

void UIController::init(void)
{
    _screen->init();
}
void UIController::renderScreen(void)
{
    Purifer_status_struct *info;
    info = _aircleanercontroller->update_and_get_whole_status();

    if (_screen->on == false)
    {
        return;
    }

    if (timeFromlastAction[interactionPhysical] < 2000)
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