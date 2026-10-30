#include "api.hpp"
#include "SDL_image.h"
#include "SDL_render.h"
#include "core.hpp"
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
}