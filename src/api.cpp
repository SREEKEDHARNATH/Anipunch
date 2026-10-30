#include "api.hpp"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "core.hpp"
#include <algorithm>
#include <iostream>
namespace core::api {
    image::image(const char* file, bool load, bool autoset){
        if (load){
            SDL_Texture* tex=IMG_LoadTexture(getRenderer(), file);
            if (!tex){
                std::cerr << IMG_GetError() << "\n";
            }
            ptr.reset(tex, SDL_DestroyTexture);
            if (autoset){
                SDL_QueryTexture(tex, NULL, NULL, &dstrect.w, &dstrect.h);
            }
        }
    }
    void image::draw(){
        if (get()){
            if (dstrect.w==0 || dstrect.h==0){
                SDL_RenderCopy(getRenderer(), get(), NULL, NULL);
            }
            else {
                SDL_RenderCopy(getRenderer(), get(), NULL, &dstrect);
            }
        }
    }
    void image::setTex(SDL_Texture* tex, bool autoset){
        if (tex){
            ptr.reset(tex, SDL_DestroyTexture);
            if (autoset){
                SDL_QueryTexture(tex, NULL, NULL, &dstrect.w, &dstrect.h);
            }
        }
        
    }
    SDL_Texture* image::get(){
        return ptr.get();
    }

    font::font(const char* file, int ptsize, bool load){
        if (load){
            auto ptr=TTF_OpenFont(file, ptsize);
            if (!ptr){
                std::cerr << TTF_GetError() << "\n";
                return;
            }
            fnt.reset(ptr, TTF_CloseFont);
        }
    }
    void font::setFont(const char* file, int ptsize){
        auto ptr=TTF_OpenFont(file, ptsize);
        if (!ptr){
            std::cerr << TTF_GetError() << "\n";
            return;
        }
        fnt.reset(ptr, TTF_CloseFont);
    }
    TTF_Font* font::get(){
        return fnt.get();
    }

    text::text(font& ft, const char* t, SDL_Colour fg) : img("", false), f("", 0,false){
        fgcol=fg;
        if (ft.get()){
            f=ft;
        }
        setText(t);
    }
    void text::draw(){
        if (img.get() && f.get()){
            img.draw();
        }
    }
    void text::setText(const char* txt){
        if (f.get()){
            t=txt;
            auto p = TTF_RenderUTF8_Blended(f.get(), txt, fgcol);
            if (!p){
                std::cerr << TTF_GetError() << "\n";
                return;
            }
            auto t=SDL_CreateTextureFromSurface(getRenderer(), p);
            if (!t){
                std::cerr << SDL_GetError() << "\n";
                return;
            }
            img.setTex(t);
        }
    }
    const char* text::getText(){
        return t.c_str();
    }

    sound::sound(const char* file){
        auto c=Mix_LoadWAV(file);
        if (!c){
            std::cerr << Mix_GetError();
            return;
        }
        ptr.reset(c, Mix_FreeChunk);
        setVol(50);
    }
    void sound::play(bool loops){
        if (get()){
            Mix_PlayChannel(-1, get(), loops);
        }
    }
    void sound::setVol(int v){
        v=std::min(v, 100);
        v=std::max(v, 0);
        
        if (get()){
            Mix_VolumeChunk(get(), v);
        }
    }
    int sound::getVol(){
        if (get()){
            return get()->volume;
        }
        return -1;
    }
    Mix_Chunk* sound::get(){
        return ptr.get();
    }

    music::music(const char* file){
        auto c=Mix_LoadMUS(file);
        if (!c){
            std::cerr << Mix_GetError();
            return;
        }
        ptr.reset(c, Mix_FreeMusic);
        setVol(50);
    }
    void music::play(bool loops){
        if (get()){
            Mix_PlayMusic(get(), loops);
        }
    }
    void music::setVol(int v){
        v=std::min(v, 100);
        v=std::max(v, 0);
        
        if (get()){
            Mix_VolumeMusic(v);
        }
    }
    int music::getVol(){
        if (get()){
            return Mix_GetMusicVolume(get());
        }
        return -1;
    }
    Mix_Music* music::get(){
        return ptr.get();
    }
}