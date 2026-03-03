#ifndef RHI_IEVENT_MANAGER__H_
#define RHI_IEVENT_MANAGER__H_

#include "Define.h"
#include "Event.hpp"
#include "IWindow.h"

enum class GAMEPAD_ID
{
    //JOYSTICK 2 
};

enum class EVENT_INPUT
{
    //KEY, MOUSE, GAMEPAD
};

enum class EVENT
{
    //PRESS, HOLD,  etc
};

class IEventManager 
{
public:
    IEventManager();
    ~IEventManager();

    bool GetKey(EVENT_INPUT key, EVENT event);

    GAMEPAD_ID AddGamepad();
    std::string_view GetGamepadName(GAMEPAD_ID id);
    std::vector<float> GetGamepadAxes(GAMEPAD_ID);
    bool GetButton(GAMEPAD_ID id, EVENT_INPUT button, EVENT event);

    Event<void(GAMEPAD_ID)> gamepadConnected;
    Event<void(GAMEPAD_ID)> gamepadDisconnected;

protected:
    
};

#endif //!RHI_IEVENT_MANAGER__H_
