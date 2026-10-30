#pragma once
#include "SDL_render.h"

namespace core {
    void run();

    SDL_Window*& getWindow();
    SDL_Renderer*& getRenderer();
    
    int getW();
    int getH();
    const char* getTitle();
}