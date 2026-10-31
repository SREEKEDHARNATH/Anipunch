#include "core.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "api.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/math_types.h"
#include "box2d/types.h"
#include "listeners.hpp"
#include "physics.hpp"

struct ki{
    physics::body body;
    core::api::image img;
    ki(physics::world& w,b2Vec2 pos) : body(w,pos,b2_kinematicBody), img("assets/images/energy-ball.png"){
        auto r = std::atan2(core::mouslistener::getY()-(pos.y*32), core::mouslistener::getX()-(pos.x*32));
        b2Rot rot=b2MakeRot(r);
        b2Body_SetTransform(body.get(), b2Body_GetPosition(body.get()), rot);
        auto f=b2Rot_GetXAxis(rot);
        int mg=20;
        b2Body_SetLinearVelocity(body.get(), {mg*f.x,mg*f.y});
    }
    void update(){
        auto pos=b2Body_GetPosition(body.get());
        img.dstrect.x=pos.x*32;
        img.dstrect.y=pos.y*32;
        img.draw();
    }
};

struct wave{
    core::api::image img;
    float r;
    wave(b2Vec2 pos) : img("assets/images/energy-wave.png"){
        r = std::atan2(core::mouslistener::getY()-(pos.y*32), core::mouslistener::getX()-(pos.x*32));
        img.dstrect.x=pos.x*32;
        img.dstrect.y=pos.y*32;
        img.dstrect.w=300;
        img.dstrect.h=50;
    }
    void update(){
        SDL_Point point={0,0};
        SDL_RenderCopyEx(core::getRenderer(), img.get(), NULL, &img.dstrect, r*180/B2_PI, &point, SDL_FLIP_NONE);
    }
};


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
        Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
        getWindow()=SDL_CreateWindow(getTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getW(), getH(), SDL_WINDOW_HIDDEN);
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

        physics::world world;
        physics::body body(world, {32,500/32.0f},b2_dynamicBody);
        body.attachBox(1, 3);

        api::image player("assets/images/player-tmp.jpg");
        player.dstrect.h*=3;

        api::image ground("assets/images/ground-tmp.jpg");;
        ground.dstrect.y=getH()-120;
        ground.dstrect.w=getW();
        ground.dstrect.h=120;
        
        physics::body bodygrd(world, {getW()/64.0f,(ground.dstrect.y+10)/32.0f},b2_staticBody);
        bodygrd.attachBox(ground.dstrect.w/32.0f, ground.dstrect.h/32.0f);;

        float base_speed_x=10,base_speed_y=-20;
        constexpr int maxjmpc=2;
        int jump_counter=maxjmpc;

        auto initial=SDL_GetTicks64();
        auto lastjmp=SDL_GetTicks64();

        std::vector<ki> energy;
        std::vector<wave> waves;
        std::vector<Uint64> wavetime;

        SDL_Rect wrldrect={0,0,getW(),getH()};
        auto lastwave=0;

        api::font font("assets/fonts/font.ttf", 24);

        api::text energybarinfo(font, "Energy:- ");
        api::text energybar(font, "100");
        energybar.img.dstrect.x=energybarinfo.img.dstrect.w;

        api::text staminabarinfo(font, "Stamina:- ");
        api::text staminabar(font, "100");

        staminabar.img.dstrect.y=energybar.img.dstrect.h+10;
        staminabarinfo.img.dstrect.y=energybar.img.dstrect.h+10;
        staminabar.img.dstrect.x=staminabarinfo.img.dstrect.w;

        api::text jumps(font, "2/2");
        jumps.img.dstrect.y=staminabar.img.dstrect.h+50;

        float curenergy=100;
        float stamina=100;

        SDL_ShowWindow(getWindow());
        while (running) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        running=false;
                        break;
                    case SDL_KEYUP:
                    case SDL_KEYDOWN:
                        keylistener::reg(e.key);
                        break;
                    case SDL_MOUSEMOTION:
                        mouslistener::reg(e.motion);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                        mouslistener::reg(e.button);
                        break;
                    case SDL_MOUSEWHEEL:
                    mouslistener::reg(e.wheel);
                    break;
                    
                }
            }
            if (keylistener::isPressed(SDLK_ESCAPE)){
                running=false;
            }
            world.simulate();
            if (keylistener::isDown(SDLK_LSHIFT)){
                base_speed_x=20;
                base_speed_y=-15;
            }
            else {
                base_speed_x=10;
                base_speed_y=-20;
            }
            if (keylistener::isDown(SDLK_LCTRL)){
                base_speed_y=-10;
                if (stamina>15){
                    if (keylistener::isPressed(SDLK_d)){
                        b2Body_ApplyLinearImpulseToCenter(body.get(), {base_speed_x,0},false);
                        stamina-=15;
                    }
                    if (keylistener::isPressed(SDLK_a)){
                        b2Body_ApplyLinearImpulseToCenter(body.get(), {-1*base_speed_x,0},false);
                        stamina-=15;
                    }
                }
            }
            else {
                if (keylistener::isDown(SDLK_d)){
                    if (stamina > 0.2f){
                        b2Body_ApplyForceToCenter(body.get(), {base_speed_x,0},false);
                        if (base_speed_x>10){
                            stamina-=0.2f;
                        }
                    }
                }
                if (keylistener::isDown(SDLK_a)){
                    if (stamina > 0.2f){
                        b2Body_ApplyForceToCenter(body.get(), {-1*base_speed_x,0},false);
                        if (base_speed_x>10){
                            stamina-=0.2f;
                        }
                    }
                }
                base_speed_y=-20;
            }
            if (keylistener::isPressed(SDLK_w)){
                auto vel = b2Body_GetLinearVelocity(body.get());
                if (jump_counter>0 && vel.y < 0.05f && stamina > 10){
                    b2Body_ApplyLinearImpulseToCenter(body.get(), {0,base_speed_y},false);
                    jump_counter--;
                    lastjmp=SDL_GetTicks64();
                    stamina-=10;
                    auto st(std::to_string(jump_counter) + "/2");
                    jumps.setText(st.c_str());
                }
            }
            else {
                if (jump_counter == 0){
                    if (SDL_GetTicks64() > (lastjmp+1000)){
                        jump_counter++;
                        lastjmp=SDL_GetTicks64();
                        jumps.setText("1/2");
                    }
                }
                else if (jump_counter == 1){
                    if (SDL_GetTicks64() > (lastjmp+4000)){
                        jump_counter++;
                        lastjmp=SDL_GetTicks64();
                        jumps.setText("2/2");
                    }
                }
            }
            if (keylistener::isDown(SDLK_e)){
                if (keylistener::isPressed(SDLK_1)){
                    if (curenergy > 4){
                        energy.emplace_back(world, b2Body_GetPosition(body.get()));
                        curenergy-=5;
                    }
                }
                if (keylistener::isPressed(SDLK_2)){
                    if (SDL_GetTicks64() > (lastwave+5000)){
                        if (curenergy > 29){
                            waves.emplace_back(b2Body_GetPosition(body.get()));
                            wavetime.push_back(SDL_GetTicks64());
                            lastwave = SDL_GetTicks64();
                            curenergy-=30;
                        }
                    }
                }
            }
            if (curenergy<100.0f){
                curenergy+=0.05f;
            }
            if (stamina<100.0f){
                stamina+=0.1f;
            }

            if ((int)(curenergy) != std::stoi(energybar.getText())){
                energybar.setText(std::to_string((int)(curenergy)).c_str());
            }
            if ((int)(stamina) != std::stoi(staminabar.getText())){
                staminabar.setText(std::to_string((int)(stamina)).c_str());
            }
            SDL_RenderClear(getRenderer());
            auto pos=b2Body_GetPosition(body.get());
            player.dstrect.x=pos.x*32;
            player.dstrect.y=pos.y*32;
            player.draw();
            ground.draw();
            for (int i=0;i<energy.size();i++){
                if (SDL_HasIntersection(&energy[i].img.dstrect, &wrldrect)){
                    energy[i].update();
                }
                else {
                    energy.erase(energy.begin()+i);
                    i--;
                }
            }
            for (int i=0;i<waves.size();i++){
                waves[i].update();
                if (SDL_GetTicks64() > wavetime[i]+4000){
                    wavetime.erase(wavetime.begin()+i);
                    waves.erase(waves.begin()+i);
                    i--;
                }
            }
            if (stamina < 1){
                std::cout << "You are out of breath bye\n";
                running=false;
            }
            jumps.draw();
            staminabarinfo.draw();
            staminabar.draw();
            energybarinfo.draw();
            energybar.draw();
            SDL_RenderPresent(getRenderer());
            
            keylistener::update();
            mouslistener::update();
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
        Mix_CloseAudio();
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
