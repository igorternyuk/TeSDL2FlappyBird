#include "resourcemanager.h"

#include <iostream>
#include <algorithm>
#ifdef DEBUG
#include <iostream>
#endif

flappyBird::ResourceManager& flappyBird::ResourceManager::getInstance()
{
    static flappyBird::ResourceManager instance;
    return instance;
}

void flappyBird::ResourceManager::loadTextureFromFile(const std::string &key,
                                              const std::string &path)
{
   my_unique_ptr<SDL_Texture> mupTexture{getTextureFromFile(path.c_str()),
                                          [](SDL_Texture *texture)
                                          {
                                                SDL_DestroyTexture(texture);
                                          }};
   textures_.insert(std::make_pair(key, std::move(mupTexture)));
}

void flappyBird::ResourceManager::loadTextureFromText(const std::string &key,
                                                      const std::string &text,
                                                      TTF_Font *pFont,
                                                      SDL_Color color)
{
    my_unique_ptr<SDL_Texture> mupTexture{getTextureFromText(text.c_str(),
                                          pFont, color),
                                           [](SDL_Texture *texture)
                                           {
                                                 SDL_DestroyTexture(texture);
                                           }};
    textures_.insert(std::make_pair(key, std::move(mupTexture)));
}

void flappyBird::ResourceManager::loadFont(const std::string &key,
                                           const std::string &path, int size)
{
    my_unique_ptr<TTF_Font> mupFont{TTF_OpenFont(path.c_str(), size),
                                    [](TTF_Font *font)
                                    {
                                        TTF_CloseFont(font);
                                    }};
    fonts_.insert(std::make_pair(key, std::move(mupFont)));
}

void flappyBird::ResourceManager::loadSound(const std::string &key,
                                            const std::string &path)
{
    my_unique_ptr<Mix_Chunk> mupSound{Mix_LoadWAV(path.c_str()),
                                      [](Mix_Chunk *sound)
                                      {
                                          Mix_FreeChunk(sound);
                                      }};
    sounds_.insert(std::make_pair(key, std::move(mupSound)));
}

void flappyBird::ResourceManager::loadMusic(const std::string &key,
                                            const std::string &path)
{
    my_unique_ptr<Mix_Music> mupMusic{Mix_LoadMUS(path.c_str()),
                                     [](Mix_Music *music)
                                     {
                                          Mix_FreeMusic(music);
                                     }};
    tracks_.insert(std::make_pair(key, std::move(mupMusic)));
}

void flappyBird::ResourceManager::removeTexture(const std::string &key)
{
    textures_.erase(textures_.find(key));
}

void flappyBird::ResourceManager::removeFont(const std::string &key)
{
    fonts_.erase(fonts_.find(key));
}

void flappyBird::ResourceManager::removeSound(const std::string &key)
{
    sounds_.erase(sounds_.find(key));
}

void flappyBird::ResourceManager::removeMusic(const std::string &key)
{
    tracks_.erase(tracks_.find(key));
}

SDL_Texture *flappyBird::ResourceManager::getTexture(const std::string &key)
{
    if(textures_.find(key) != textures_.end())
        return textures_[key].get();
    else
        return nullptr;
}

TTF_Font *flappyBird::ResourceManager::getFont(const std::string &key)
{
    if(fonts_.find(key) != fonts_.end())
        return fonts_[key].get();
    else
        return nullptr;
}

Mix_Chunk *flappyBird::ResourceManager::getSound(const std::string &key)
{
    if(sounds_.find(key) != sounds_.end())
        return sounds_[key].get();
    else
        return nullptr;
}

Mix_Music *flappyBird::ResourceManager::getMusic(const std::string &key)
{
    if(tracks_.find(key) != tracks_.end())
        return tracks_[key].get();
    else
        return nullptr;
}

//Private constructor
flappyBird::ResourceManager::ResourceManager():
    upWindow_{SDL_CreateWindow(WINDOW_TITLE.c_str(), WINDOW_X, WINDOW_Y,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN),
              [](SDL_Window *window){ SDL_DestroyWindow(window); }
},
upRenderer_{SDL_CreateRenderer(upWindow_.get(), -1,
                               SDL_RENDERER_ACCELERATED |
                               SDL_RENDERER_PRESENTVSYNC),
            [](SDL_Renderer *renderer){ SDL_DestroyRenderer(renderer); }
           }
{
fillVectorOfDigits();
}

void flappyBird::ResourceManager::loadDefaultFont()
{
    static const std::string pathToDefaultFont
    {"Resources/Fonts/orbitron-light.ttf"};
    static const int defaultFontSize{32};
    this->loadFont("default", pathToDefaultFont, defaultFontSize);
}

flappyBird::ResourceManager::~ResourceManager()
{
    vupDigitTextures_.clear();
}

void flappyBird::ResourceManager::fillVectorOfDigits()
{
    loadDefaultFont();
    for(auto i = 0; i < NUM_DIGIT_TEXTURES; ++i)
    {
        vupDigitTextures_.emplace_back(getTextureFromText(std::to_string(i).c_str(),
                                                          fonts_["default"].get(),
                                                          colorDigits_),
                                       [](SDL_Texture *texture){
            SDL_DestroyTexture(texture); });
    }
}

SDL_Texture* flappyBird::ResourceManager::getTextureFromFile(const char *fileName)
{
    auto pSurface = IMG_Load(fileName);
    if(!pSurface)
        throw std::runtime_error("Error while loading image:" +
                                 std::string(fileName) + " - " + SDL_GetError());
    auto pTexture = SDL_CreateTextureFromSurface(upRenderer_.get(), pSurface);
    SDL_FreeSurface(pSurface);
    pSurface = nullptr;
    return pTexture;
}

SDL_Texture* flappyBird::ResourceManager::getTextureFromText(const char *text,
                                                         TTF_Font *font,
                                                         SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(upRenderer_.get(),
                                                        surface);
    SDL_FreeSurface(surface);
    return texture;
}

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

}

flappyBird::SDLInitObject::~SDLInitObject()
{
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
