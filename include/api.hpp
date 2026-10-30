#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"
#include <memory>
namespace core::api {
    class image{
        std::shared_ptr<SDL_Texture> ptr;
        public:
            SDL_Rect dstrect={0};
            image(const char* file, bool load=true, bool autoset=true);
            void draw();
            void setTex(SDL_Texture* tex, bool autoset=true);
            SDL_Texture* get();
    };
}
