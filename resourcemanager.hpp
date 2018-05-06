#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "flappy_bird_constants.hpp"

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <stdexcept>

namespace flappyBird
{
    template<typename T>
    using my_unique_ptr = std::unique_ptr<T,std::function<void(T*)>>;

    class ResourceManager
    {
    public:
        explicit ResourceManager(SDL_Renderer* renderer);
        ~ResourceManager();
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        void loadTextureFromFile(const std::string& key, const std::string& path);
        void loadTextureFromText(const std::string& key, const std::string& text,
                                 TTF_Font *pFont, SDL_Color color);
        void loadFont(const std::string& key, const std::string& path, int size);
        void loadSound(const std::string& key, const std::string& path);
        void loadMusic(const std::string& key, const std::string& path);

        void removeTexture(const std::string& key);
        void removeFont(const std::string& key);
        void removeSound(const std::string& key);
        void removeMusic(const std::string& key);

        SDL_Texture *getTexture(const std::string& key) const;
        inline auto digitTexture(int i) const noexcept
        { return vupDigitTextures_.at(i).get(); }
        TTF_Font* getFont(const std::string& key) const;
        Mix_Chunk* getSound(const std::string& key) const;
        Mix_Music* getMusic(const std::string& key) const;
        SDL_Renderer *getRenderer() const;
        void setRenderer(SDL_Renderer *renderer);

    private:
        enum { NUM_DIGIT_TEXTURES = 10 };
        const SDL_Color colorDigits_{255, 0, 0, 255};
        SDL_Renderer *renderer_;
        std::vector<my_unique_ptr<SDL_Texture>> vupDigitTextures_;
        std::map<std::string, my_unique_ptr<SDL_Texture>> textures_;
        std::map<std::string, my_unique_ptr<TTF_Font>> fonts_;
        std::map<std::string, my_unique_ptr<Mix_Music>> tracks_;
        std::map<std::string, my_unique_ptr<Mix_Chunk>> sounds_;

    private:
        void loadDefaultFont();
        void fillVectorOfDigits();
        SDL_Texture *getTextureFromText(const char *text, TTF_Font *font,
                                        SDL_Color color);
        SDL_Texture *getTextureFromFile(const char *fileName);
    };
}
