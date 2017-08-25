#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include "flappy_bird_constants.h"

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
        inline SDL_Window* window() const { return upWindow_.get(); }
        inline SDL_Renderer* renderer() const { return upRenderer_.get(); }
        inline SDL_Texture* birdTexture() const { return upBirdSpriteTexture_.get(); }
        inline SDL_Texture* tubeTexture() const { return upTubeTexture_.get(); }
        inline SDL_Texture* digitTexture(int i) const { return vupDigitTextures_.at(i).get(); }
        inline TTF_Font* fontBig() const { return upFontBig_.get(); }
        inline TTF_Font* fontSmall() const { return upFontSmall_.get(); }
        inline SDL_Texture* gameScoreTexture() const { return upGameScoreTexture_.get(); }
        inline SDL_Texture* gameOverTexture() const { return upGameOverTexture_.get(); }
        inline SDL_Texture* gamePausedTexture() const { return upGamePausedTexture_.get(); }
        inline Mix_Music* backgroundMusic() const { return upBackgroundMusic_.get(); }
        inline Mix_Chunk* collectPointSound() const { return upCollectPointSound_.get(); }
        inline Mix_Chunk* gameOverSound() const { return upGameOverSound_.get(); }

    private:
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
        explicit ResourceManager();
        void fillVectorOfDigits();
        SDL_Texture *getTextureFromText(const char *text, TTF_Font *font, SDL_Color color);
        SDL_Texture *loadTexture(const char *fileName);
    };
}
