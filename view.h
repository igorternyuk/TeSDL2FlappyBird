#pragma once
#include <string>
#include <memory>
#include <SDL2/SDL_thread.h>
#include "modellistener.h"
#include "flappy_bird_constants.h"
#include "resourcemanager.h"

/*Хочу применить класс менеджер ресурсов
 Использую прозрачный формат спрайтов и спайт-анимации
 Использую звуки и шрифты и все по паттерну MVC*/

//Делаем два потока один для обновления игровой модели:
//1) - движение труб и падение птички
//2) - анимация птички - птичка машет крыльями

namespace flappyBird {
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
        bool isRunning_{true};
        mutable bool draw_{true};
        //ResourceManager resourceManager_;
        ResourceManager &resourceManager_{ResourceManager::getInstance()};
        std::shared_ptr<Model> spModel_;
        std::unique_ptr<Controller> upController_;
        SDL_Thread *pBirdAnimThread_;
        int birdFrameX_{0};
        bool soundEnabled_{true};
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

