#pragma once
#include <irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver();
    virtual bool OnEvent(const SEvent& event);
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

private:
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

