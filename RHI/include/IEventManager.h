#ifndef RHI_IEVENT_MANAGER__H_
#define RHI_IEVENT_MANAGER__H_

#include "Define.h"
#include "Event.hpp"
#include "IWindow.h"

enum class GAMEPAD_ID
{

};

enum class EVENT_INPUT
{
         
};

enum class EVENT
{
    
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

#endif //!RHI_IENT_MANAGER__H_
