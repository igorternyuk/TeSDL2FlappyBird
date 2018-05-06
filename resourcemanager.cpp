#include "resourcemanager.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>

#ifdef DEBUG
#include <iostream>
#endif

void flappyBird::ResourceManager::loadTextureFromFile(const std::string &key,
                                              const std::string &path)
{
   my_unique_ptr<SDL_Texture> mupTexture{getTextureFromFile(path.c_str()),
                                         SDL_DestroyTexture };
   textures_.insert(std::make_pair(key, std::move(mupTexture)));
}

void flappyBird::ResourceManager::loadTextureFromText(const std::string &key,
                                                      const std::string &text,
                                                      TTF_Font *pFont,
                                                      SDL_Color color)
{
    my_unique_ptr<SDL_Texture> mupTexture{ getTextureFromText(text.c_str(),
                                          pFont, color),
                                          SDL_DestroyTexture };
    auto inserted = textures_.insert(std::make_pair(key, std::move(mupTexture)));
    assert(inserted.second);
}

void flappyBird::ResourceManager::loadFont(const std::string &key,
                                           const std::string &path, int size)
{
    my_unique_ptr<TTF_Font> mupFont{ TTF_OpenFont(path.c_str(), size),
                                    TTF_CloseFont };
    auto inserted = fonts_.insert(std::make_pair(key, std::move(mupFont)));
    assert(inserted.second);
}

void flappyBird::ResourceManager::loadSound(const std::string &key,
                                            const std::string &path)
{
    my_unique_ptr<Mix_Chunk> mupSound{ Mix_LoadWAV(path.c_str()),
                                      Mix_FreeChunk };
    auto inserted = sounds_.insert(std::make_pair(key, std::move(mupSound)));
    assert(inserted.second);
}

void flappyBird::ResourceManager::loadMusic(const std::string &key,
                                            const std::string &path)
{
    my_unique_ptr<Mix_Music> mupMusic{Mix_LoadMUS(path.c_str()),
                                     Mix_FreeMusic};
    auto inserted = tracks_.insert(std::make_pair(key, std::move(mupMusic)));
    assert(inserted.second);
}

void flappyBird::ResourceManager::removeTexture(const std::string &key)
{
    auto found = textures_.find(key);
    assert(found != textures_.end());
    textures_.erase(found);
}

void flappyBird::ResourceManager::removeFont(const std::string &key)
{
    auto found = fonts_.find(key);
    assert(found != fonts_.end());
    fonts_.erase(found);
}

void flappyBird::ResourceManager::removeSound(const std::string &key)
{
    auto found = sounds_.find(key);
    assert(found != sounds_.end());
    sounds_.erase(found);
}

void flappyBird::ResourceManager::removeMusic(const std::string &key)
{
    auto found = tracks_.find(key);
    assert(found != tracks_.end());
    tracks_.erase(found);
}

SDL_Texture *flappyBird::ResourceManager::getTexture(const std::string &key) const
{
    auto found = textures_.find(key);
    assert(found != textures_.end());
    return textures_.at(key).get();
}

TTF_Font *flappyBird::ResourceManager::getFont(const std::string &key) const
{
    auto found = fonts_.find(key);
    assert(found != fonts_.end());
    return fonts_.at(key).get();
}

Mix_Chunk *flappyBird::ResourceManager::getSound(const std::string &key) const
{
    auto found = sounds_.find(key);
    assert(found != sounds_.end());
    return sounds_.at(key).get();
}

Mix_Music *flappyBird::ResourceManager::getMusic(const std::string &key) const
{
    auto found = tracks_.find(key);
    assert(found != tracks_.end());
    return tracks_.at(key).get();
}

SDL_Renderer* flappyBird::ResourceManager::getRenderer() const
{
    return renderer_;
}

void flappyBird::ResourceManager::setRenderer(SDL_Renderer *renderer)
{
    renderer_ = renderer;
}

//Private constructor
flappyBird::ResourceManager::ResourceManager(SDL_Renderer *renderer):
    renderer_{ renderer }
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
                                       SDL_DestroyTexture);
    }
}

SDL_Texture* flappyBird::ResourceManager::getTextureFromFile(const char *fileName)
{
    auto pSurface = IMG_Load(fileName);
    if(!pSurface)
        throw std::runtime_error("Error while loading image:" +
                                 std::string(fileName) + " - " + SDL_GetError());
    auto pTexture = SDL_CreateTextureFromSurface(renderer_, pSurface);
    SDL_FreeSurface(pSurface);
    pSurface = nullptr;
    return pTexture;
}

SDL_Texture* flappyBird::ResourceManager::getTextureFromText(const char *text,
                                                         TTF_Font *font,
                                                         SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    return texture;
}


