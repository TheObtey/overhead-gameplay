#ifndef ORE_WINDOW__H_
#define ORE_WINDOW__H_

#include "IWindow.h"

class Window : public IWindow
{
public:
    Window(int width, int height, std::string name);
    ~Window();

    virtual void Close() override;

protected:
    virtual void Open() override;
};


#endif //!ORE_WINDOW__H_