#pragma once
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput9_1_0.lib")

struct Pad {
    XINPUT_STATE s{};
    WORD previous = 0;
    bool connected = false;

    void Update() {
        previous = s.Gamepad.wButtons;
        ZeroMemory(&s, sizeof(s));
        connected = XInputGetState(0, &s) == ERROR_SUCCESS;
    }
    bool Down(WORD b) const { return connected && (s.Gamepad.wButtons & b); }
    bool Pressed(WORD b) const { return connected && (s.Gamepad.wButtons & b) && !(previous & b); }
    bool LT() const { return connected && s.Gamepad.bLeftTrigger > 80; }
    bool RT() const { return connected && s.Gamepad.bRightTrigger > 80; }
};

extern Pad pad;
