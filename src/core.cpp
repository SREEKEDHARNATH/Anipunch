#include "core.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "api.hpp"
#include <iostream>
#include <utility>

namespace core{
    std::pair<bool, const char*> init() {
        int img_flags = IMG_INIT_PNG;
        int sound_flags = MIX_INIT_MP3;
        if (SDL_Init(SDL_INIT_EVERYTHING)){
            return {false, SDL_GetError()};
        }
        if ((IMG_Init(img_flags) & img_flags) != img_flags){
            return {false, IMG_GetError()};
        }
        if (TTF_Init()){
            return {false, IMG_GetError()};
        }
        if ((Mix_Init(sound_flags) & sound_flags) != sound_flags){
            return {false, Mix_GetError()};
        }
        getWindow()=SDL_CreateWindow(getTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getW(), getH(), 0);
        if (!getWindow()){
            return {false, SDL_GetError()};
        }
        getRenderer()=SDL_CreateRenderer(getWindow(),-1,0);
        if (!getRenderer()){
            return {false, SDL_GetError()};
        }
        return {true, "Great"};
    }
    void quit();
    void run(){
        auto i=init();
        if (!i.first){
            std::cerr << i.second << "\n";
            return;
        }
        
        SDL_Event e;
        bool running=true;

        while (running) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        running=false;
                        break;
                }
            }
            SDL_RenderClear(getRenderer());
            SDL_RenderPresent(getRenderer());
            
            SDL_Delay(16);
        }
    }
    void quit(){
        if (getRenderer()){
            SDL_DestroyRenderer(getRenderer());
        }
        if (getWindow()){
            SDL_DestroyWindow(getWindow());
        }
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    
    SDL_Window*& getWindow(){
        static SDL_Window* window=nullptr;
        return window;
    }
    SDL_Renderer*& getRenderer(){
        static SDL_Renderer* renderer=nullptr;
        return renderer;
    }
        
    int getW(){
        return 1280;
    }
    int getH(){
        return 720;
    }
    const char* getTitle(){
        return "Anipunch";
    }
}
