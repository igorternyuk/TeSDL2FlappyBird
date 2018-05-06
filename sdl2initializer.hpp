#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "flappy_bird_constants.hpp"

#include <vector>
#include <string>

namespace flappyBird
{
    class SDLInitObject
    {
    public:
        explicit SDLInitObject();
        ~SDLInitObject();
        int screenWidth, screenHeight;
    private:
        enum
        {
            ERROR_SDL_INIT = 0,
            ERROR_TTF_LOADING = 1,
            ERROR_AUDIO_LOADING = 2
        };
        const std::vector<std::string> vErrors_
        {
            "Error while loading SDL: ",
            "Error while loading TTF module: ",
            "Error while opening audio module: "
        };
    };
}

