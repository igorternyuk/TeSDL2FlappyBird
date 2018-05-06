#include "sdl2initializer.hpp"
#include <stdexcept>

#ifdef DEBUG
#include <iostream>
#endif

flappyBird::SDLInitObject::SDLInitObject()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(vErrors_[ERROR_SDL_INIT] +
                                 std::string(SDL_GetError()));
    if(TTF_Init() < 0)
        throw std::runtime_error(vErrors_[ERROR_TTF_LOADING] +
                                 std::string(TTF_GetError()));
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throw std::runtime_error(vErrors_[ERROR_AUDIO_LOADING]+
                                 std::string(Mix_GetError()));
    SDL_DisplayMode dm;
    if(SDL_GetDesktopDisplayMode(0, &dm)){
        throw std::runtime_error(std::string(SDL_GetError()));
    }
    screenWidth = dm.w;
    screenHeight = dm.h;
#ifdef DEBUG
    std::cout << screenWidth << " X " << screenHeight << std::endl;
#endif
}

flappyBird::SDLInitObject::~SDLInitObject()
{
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
