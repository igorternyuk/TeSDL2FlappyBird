#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <memory>

namespace iat_fb {
    class Model;
    class Controller
    {
    public:
        explicit Controller(std::shared_ptr<Model> model);
        ~Controller();
        void startNewGame();
        void togglePause();
        void birdUp();
        void birdDown();
        inline void stopProgram() { isGameRunning_ = false;}
    private:
        enum { DELAY = 70};
        bool isGameRunning_{true};
        std::shared_ptr<Model> spModel_;
        SDL_Thread *pThreadModelStateUpdater_;
        static int theadFuncWrapper(void *data);
        int threadFunc();
    };
}



