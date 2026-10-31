#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
namespace core::keylistener{
    void reg(SDL_KeyboardEvent k);

    bool isPressed(SDL_KeyCode k);
    bool isPressed(SDL_Scancode sc);
    
    bool isDown(SDL_KeyCode k);
    bool isDown(SDL_Scancode sc);
    
    void update();
}
namespace core::mouslistener {
    void reg(SDL_MouseMotionEvent m);
    void reg(SDL_MouseButtonEvent b);
    void reg(SDL_MouseWheelEvent w);
    
    bool isPressed(int b);
    bool isDown(int b);
    
    int getX();
    int getY();
    
    float getScrlX();
    float getScrlY();
    
    void update();
}