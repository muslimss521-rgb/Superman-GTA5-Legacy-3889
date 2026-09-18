#include "script.h"
#include "controller.h"

static bool gSuperman=false;
static bool gFlight=false;
static bool gSpeed=false;

static void Notify(const char* msg)
{
    UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
    UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)msg);
    UI::_DRAW_NOTIFICATION(false, false);
}

static void SetPowers(Ped p, bool on)
{
    ENTITY::SET_ENTITY_INVINCIBLE(p,on);
    PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(),on);
    PED::SET_PED_CAN_RAGDOLL(p,!on);
    PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(p,!on);
}

static void Flight(Ped p)
{
    if(!gFlight) return;
    Vector3 pos=ENTITY::GET_ENTITY_COORDS(p,true);
    Vector3 f=ENTITY::GET_ENTITY_FORWARD_VECTOR(p);
    float speed=pad.Down(XINPUT_GAMEPAD_RIGHT_SHOULDER)?2.5f:0.8f;

    if(pad.Down(XINPUT_GAMEPAD_DPAD_UP)) {
        pos.x+=f.x*speed; pos.y+=f.y*speed; pos.z+=f.z*speed;
    }
    if(pad.Down(XINPUT_GAMEPAD_DPAD_DOWN)) {
        pos.x-=f.x*speed; pos.y-=f.y*speed; pos.z-=f.z*speed;
    }
    if(pad.Down(XINPUT_GAMEPAD_A)) pos.z+=speed;
    if(pad.Down(XINPUT_GAMEPAD_B)) pos.z-=speed;

    ENTITY::SET_ENTITY_VELOCITY(p,0,0,0);
    ENTITY::SET_ENTITY_COORDS_NO_OFFSET(p,pos.x,pos.y,pos.z,true,true,true);
}

static void SuperJump(Ped p)
{
    if(!pad.Pressed(XINPUT_GAMEPAD_Y)) return;
    Vector3 f=ENTITY::GET_ENTITY_FORWARD_VECTOR(p);
    ENTITY::APPLY_FORCE_TO_ENTITY(
        p,1,f.x*10.0f,f.y*10.0f,40.0f,
        0,0,0,0,false,true,true,false,true);
}

static void SuperPunch(Ped p)
{
    if(!pad.Down(XINPUT_GAMEPAD_X)) return;
    Vector3 a=ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(p,0.0f,2.2f,0.7f);
    FIRE::ADD_EXPLOSION(a.x,a.y,a.z,0,0.25f,false,true,0.0f,false);
}

static void HeatVision(Ped p)
{
    if(!pad.Down(XINPUT_GAMEPAD_LEFT_SHOULDER)) return;
    Vector3 a=ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(p,0.0f,0.7f,0.65f);
    Vector3 f=ENTITY::GET_ENTITY_FORWARD_VECTOR(p);
    Vector3 b={a.x+f.x*90.0f,a.y+f.y*90.0f,a.z+f.z*90.0f};
    GRAPHICS::DRAW_LINE(a.x,a.y,a.z,b.x,b.y,b.z,255,0,0,255);
    FIRE::ADD_EXPLOSION(b.x,b.y,b.z,2,0.02f,false,true,0.0f,false);
}

static void FreezeBreath(Ped p)
{
    if(!pad.LT()) return;
    Vector3 a=ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(p,0.0f,0.7f,0.65f);
    Vector3 f=ENTITY::GET_ENTITY_FORWARD_VECTOR(p);
    Vector3 b={a.x+f.x*35.0f,a.y+f.y*35.0f,a.z+f.z*35.0f};
    GRAPHICS::DRAW_LINE(a.x,a.y,a.z,b.x,b.y,b.z,170,220,255,255);
}

static void SuperSpeed(Ped p)
{
    if(!gSpeed || !pad.Down(XINPUT_GAMEPAD_DPAD_UP)) return;
    Vector3 f=ENTITY::GET_ENTITY_FORWARD_VECTOR(p);
    ENTITY::SET_ENTITY_VELOCITY(p,f.x*16.0f,f.y*16.0f,f.z*2.0f);
}

void ScriptMain()
{
    while(true)
    {
        pad.Update();
        Ped p=PLAYER::PLAYER_PED_ID();

        if(pad.Pressed(XINPUT_GAMEPAD_START))
        {
            gSuperman=!gSuperman;
            SetPowers(p,gSuperman);
            Notify(gSuperman ? "SUPERMAN: ON" : "SUPERMAN: OFF");
            if(!gSuperman){gFlight=false;gSpeed=false;}
        }

        if(gSuperman)
        {
            if(pad.Pressed(XINPUT_GAMEPAD_RIGHT_THUMB))
            {
                gFlight=!gFlight;
                Notify(gFlight ? "FLIGHT: ON" : "FLIGHT: OFF");
            }
            if(pad.Pressed(XINPUT_GAMEPAD_LEFT_THUMB))
            {
                gSpeed=!gSpeed;
                Notify(gSpeed ? "SUPER SPEED: ON" : "SUPER SPEED: OFF");
            }

            SetPowers(p,true);
            SuperJump(p);
            SuperPunch(p);
            HeatVision(p);
            FreezeBreath(p);
            Flight(p);
            SuperSpeed(p);
        }

        WAIT(0);
    }
}
