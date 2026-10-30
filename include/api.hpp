#pragma once

#include "SDL_mixer.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include <memory>
#include <string>
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

    class font{
        std::shared_ptr<TTF_Font> fnt;
        public:
            font(const char* file, int ptsize, bool load=true);
            void setFont(const char* file, int ptsize);
            TTF_Font* get();
    };
    class text{
        font f;
        SDL_Colour fgcol;
        std::string t;
        public:
            image img;
            text(font& ft, const char* txt, SDL_Colour fg={255,255,255,255});
            void draw();
            void setText(const char* txt);
            const char* getText();
    };
    class sound{
        std::shared_ptr<Mix_Chunk> ptr;
        public:
            sound(const char* file);
            void play(bool loops=false);
            void setVol(int v);
            int getVol();
            Mix_Chunk* get();
    };
    class music{
        std::shared_ptr<Mix_Music> ptr;
        public:
            music(const char* file);
            void play(bool loops=false);
            void pause();
            void resume();
            void halt();
            void setVol(int v);
            int getVol();
            Mix_Music* get();
    };
    
}
