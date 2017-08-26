#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "flappy_bird_constants.h"

#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>

namespace iat_fb {
    template<typename T>
    using my_unique_ptr = std::unique_ptr<T,std::function<void(T*)>>;

    class SDLInitObject
    {
    public:
        explicit SDLInitObject();
        ~SDLInitObject();
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

    class ResourceManager
    {
    public:
        static ResourceManager& getInstance();
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        inline auto window() const noexcept { return upWindow_.get(); }
        inline auto renderer() const noexcept { return upRenderer_.get(); }
        inline auto birdTexture() const noexcept { return upBirdSpriteTexture_.get(); }
        inline auto tubeTexture() const noexcept { return upTubeTexture_.get(); }
        inline auto digitTexture(int i) const noexcept { return vupDigitTextures_.at(i).get(); }
        inline auto fontBig() const noexcept { return upFontBig_.get(); }
        inline auto fontSmall() const noexcept { return upFontSmall_.get(); }
        inline auto gameScoreTexture() const noexcept { return upGameScoreTexture_.get(); }
        inline auto gameOverTexture() const noexcept { return upGameOverTexture_.get(); }
        inline auto gamePausedTexture() const noexcept { return upGamePausedTexture_.get(); }
        inline auto backgroundMusic() const noexcept { return upBackgroundMusic_.get(); }
        inline auto collectPointSound() const noexcept { return upCollectPointSound_.get(); }
        inline auto gameOverSound() const noexcept { return upGameOverSound_.get(); }

    private:
        explicit ResourceManager();
        SDLInitObject initializatorOfSDL2_;
        enum {
            NUM_DIGIT_TEXTURES = 10,
            BIG_FONT_SIZE = 100,
            SMALL_FONT_SIZE = 32,
            BIRD_SPRITE = 0,
            TUBE_IMAGE = 1,
            FONT_PATH = 2,
            MUSIC_BACKGROUND = 3,
            SOUND_COLLECT_POINT = 4,
            SOUND_GAME_OVER = 5
        };
        const std::vector<const char*> vPath_
        {
            "Resources/Images/birdSprite4.png",
            "Resources/Images/tubo.png",
            "Resources/Fonts/orbitron-light.ttf",
            "Resources/Sounds/background.mp3",
            "Resources/Sounds/collect_point.wav",
            "Resources/Sounds/bird_death.wav",
        };

        const SDL_Color colorDigits_{255, 0, 0, 255};
        my_unique_ptr<SDL_Window> upWindow_;             //Main SDL window
        my_unique_ptr<SDL_Renderer> upRenderer_;         //Renderer of main window
        //textures
        my_unique_ptr<SDL_Texture> upBirdSpriteTexture_;
        my_unique_ptr<SDL_Texture> upTubeTexture_;
        //fonts
        my_unique_ptr<TTF_Font> upFontBig_, upFontSmall_;
        //font textures
        my_unique_ptr<SDL_Texture> upGameScoreTexture_;
        my_unique_ptr<SDL_Texture> upGameOverTexture_;
        my_unique_ptr<SDL_Texture> upGamePausedTexture_;
        std::vector<my_unique_ptr<SDL_Texture>> vupDigitTextures_;
        //sounds and music
        my_unique_ptr<Mix_Music> upBackgroundMusic_;
        my_unique_ptr<Mix_Chunk> upCollectPointSound_;
        my_unique_ptr<Mix_Chunk> upGameOverSound_;

        void fillVectorOfDigits();
        SDL_Texture *getTextureFromText(const char *text, TTF_Font *font, SDL_Color color);
        SDL_Texture *loadTexture(const char *fileName);
    };
}
