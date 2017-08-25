#include "resourcemanager.h"
#include <iostream>
#include <algorithm>
#ifdef DEBUG
#include <iostream>
#endif

iat_fb::ResourceManager& iat_fb::ResourceManager::getInstance()
{
    static iat_fb::ResourceManager instance;
    return instance;
}

/*        enum {
            NUM_DIGIT_TEXTURES = 10,
            BIG_FONT_SIZE = 100,
            SMALL_FONT_SIZE = 32,
            BIRD_SPRITE = 0,
            TUBE_IMAGE = 1,
            FONT_PATH = 2,
            MUSIC_BACKGROUND = 3,
            SOUND_COLLECT_POINT = 4,
            SOUND_GAME_OVER = 5,
            ERROR_IMAGE_LOADING = 0,
            ERROR_SDL_INIT = 1,
            ERROR_TTF_LOADING = 2,
            ERROR_AUDIO_LOADING = 3
        };*/

iat_fb::ResourceManager::ResourceManager():
    upWindow_{ SDL_CreateWindow(WINDOW_TITLE.c_str(), WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN),
             [](SDL_Window *window){ SDL_DestroyWindow(window); }
    },
    upRenderer_{SDL_CreateRenderer(upWindow_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
              [](SDL_Renderer *renderer){ SDL_DestroyRenderer(renderer); }
    },
    upBirdSpriteTexture_{loadTexture(vPath_[BIRD_SPRITE]),
                       [](SDL_Texture *texture){ SDL_DestroyTexture(texture); }
    },
    upTubeTexture_{loadTexture(vPath_[TUBE_IMAGE]),
                 [](SDL_Texture *texture){ SDL_DestroyTexture(texture); }
    },
    upFontBig_{TTF_OpenFont(vPath_[FONT_PATH], BIG_FONT_SIZE),
             [](TTF_Font *font){ TTF_CloseFont(font); }
    },
    upFontSmall_{TTF_OpenFont(vPath_[FONT_PATH], SMALL_FONT_SIZE),
             [](TTF_Font *font){ TTF_CloseFont(font); }
    },
    upGameScoreTexture_{getTextureFromText("SCORE: ", upFontSmall_.get(), {0, 0, 255, 255}),
                      [](SDL_Texture *texture){ SDL_DestroyTexture(texture); }
    },
    upGameOverTexture_{getTextureFromText("GAME OVER!!!", upFontBig_.get(), {255, 0, 0, 255}),
                      [](SDL_Texture *texture){ SDL_DestroyTexture(texture); }
    },
    upGamePausedTexture_{getTextureFromText("PAUSED", upFontBig_.get(), {0, 127, 0, 255}),
                      [](SDL_Texture *texture){ SDL_DestroyTexture(texture); }
    },
    upBackgroundMusic_{Mix_LoadMUS(vPath_[MUSIC_BACKGROUND]/*"Resources/Sounds/background.mp3"*/),
                      [](Mix_Music *music){ Mix_FreeMusic(music); }
    },
    upCollectPointSound_{Mix_LoadWAV(vPath_[SOUND_COLLECT_POINT]/*"Resources/Sounds/collect_point.wav"*/),
                      [](Mix_Chunk *sound){ Mix_FreeChunk(sound); }
    },
    upGameOverSound_{Mix_LoadWAV(vPath_[SOUND_GAME_OVER]/*"Resources/Sounds/bird_death.wav"*/),
                      [](Mix_Chunk *sound){ Mix_FreeChunk(sound); }
    }
{
    fillVectorOfDigits();
}

void iat_fb::ResourceManager::fillVectorOfDigits()
{
    for(auto i = 0; i < NUM_DIGIT_TEXTURES; ++i)
    {
        vupDigitTextures_.emplace_back(getTextureFromText(std::to_string(i).c_str(), upFontSmall_.get(), colorDigits_),
                                       [](SDL_Texture *texture){ SDL_DestroyTexture(texture); });
    }
}

SDL_Texture* iat_fb::ResourceManager::loadTexture(const char *fileName)
{
    auto pSurface = IMG_Load(fileName);
    if(!pSurface)
    {
        throw std::runtime_error("Error while loading image:" + std::string(fileName) + " - " + SDL_GetError());
    }
    auto pTexture = SDL_CreateTextureFromSurface(upRenderer_.get(), pSurface);
    SDL_FreeSurface(pSurface);
    return pTexture;
}

SDL_Texture* iat_fb::ResourceManager::getTextureFromText(const char *text, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(upRenderer_.get(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

iat_fb::SDLInitObject::SDLInitObject()
{
    if(SDL_Init((SDL_INIT_EVERYTHING)) < 0)
        throw std::runtime_error(vErrors_[ERROR_SDL_INIT] + std::string(SDL_GetError()));
    if(TTF_Init() < 0)
        throw std::runtime_error(vErrors_[ERROR_TTF_LOADING] + std::string(TTF_GetError()));
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throw std::runtime_error(vErrors_[ERROR_AUDIO_LOADING]+ std::string(Mix_GetError()));

}

iat_fb::SDLInitObject::~SDLInitObject()
{
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
