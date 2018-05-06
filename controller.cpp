#include "controller.hpp"

#include "model.hpp"

#ifdef DEBUG
#include <iostream>
#endif


flappyBird::Controller::Controller(std::shared_ptr<Model> model):
    spModel_{model}
{
    pThreadModelStateUpdater_ = SDL_CreateThread(theadFuncWrapper, "ModelUpdater", this);
}

flappyBird::Controller::~Controller()
{
    //SDL_WaitThread(pThreadModelStateUpdater_, NULL);
}

void flappyBird::Controller::startNewGame()
{
    spModel_->startNewGame();
}

void flappyBird::Controller::togglePause()
{
    spModel_->togglePause();
}

void flappyBird::Controller::birdUp()
{
    spModel_->pitchUpBird();
}

void flappyBird::Controller::birdDown()
{
    spModel_->diveBird();
}

void flappyBird::Controller::stopProgram()
{
    isGameRunning_ = false;
    SDL_WaitThread(pThreadModelStateUpdater_, NULL);
}

int flappyBird::Controller::theadFuncWrapper(void *data)
{
    Controller *self = static_cast<Controller*>(data);
    return self->threadFunc();
}

int flappyBird::Controller::threadFunc()
{
    while(isGameRunning_)
    {
        SDL_Delay( DELAY);
        if(spModel_->getGameState() == Model::GameState::PLAY)
            spModel_->step();
    }
    return 0;
}
