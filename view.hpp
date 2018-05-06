#pragma once

#include <SDL2/SDL_thread.h>
#include "sdl2initializer.hpp"
#include "modellistener.hpp"
#include "flappy_bird_constants.hpp"
#include "resourcemanager.hpp"
#include <memory>
#include <string>

namespace flappyBird
{
    class Model;
    class Controller;
    class View: public ModelListener
    {
    public:
        explicit View(std::shared_ptr<Model> model);
        ~View();
        void run();
        virtual void updateView();
        virtual void collectedPoint();
        virtual void gameOver();
    private:
        enum
        {
            BIRD_ANIMATION_DELAY_MS = 200,
            BIRD_SPRITE_NUM_FRAMES_X = 4,
            GAME_SCORE_POSITION_X = 10,
            GAME_SCORE_POSITION_Y = WINDOW_HEIGHT - 50,
            MARGIN_BETWEEN_LETTERS = 2
        };

        SDLInitObject initializatorOfSDL2_;
        my_unique_ptr<SDL_Window> upWindow_;             //Main SDL window
        my_unique_ptr<SDL_Renderer> upRenderer_;         //Renderer of main window
        ResourceManager resourceManager_;

        std::shared_ptr<Model> spModel_;
        std::unique_ptr<Controller> upController_;
        SDL_Thread *pBirdAnimThread_;
        int birdFrameX_{0};
        bool soundEnabled_{true};
        bool isRunning_{true};
        mutable bool draw_{true};
    private:
        static int birdAnimaThreadFuncWrapper(void *data);
        int birdAnimThreadFunc();
        void loadTextures();
        void loadFonts();
        void loadSoundsAndMusic();
        void inputPhase();
        void drawPhase() const;
        void drawBird() const;
        void drawTubes() const;
        void drawGameStatus() const;
        void drawScore() const;
        void toggleGameSounds();
    };
}

