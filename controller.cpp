#include "controller.h"

#include "model.h"

#ifdef DEBUG
#include <iostream>
#endif


iat_fb::Controller::Controller(std::shared_ptr<Model> model):
    spModel_{model}
{
    pThreadModelStateUpdater_ = SDL_CreateThread(theadFuncWrapper, "ModelUpdater", this);
}

iat_fb::Controller::~Controller()
{
    //SDL_WaitThread(pThreadModelStateUpdater_, NULL);
}

void iat_fb::Controller::startNewGame()
{
    spModel_->startNewGame();
}

void iat_fb::Controller::togglePause()
{
    spModel_->togglePause();
}

void iat_fb::Controller::birdUp()
{
    spModel_->pitchUpBird();
}

void iat_fb::Controller::birdDown()
{
    spModel_->diveBird();
}

void iat_fb::Controller::stopProgram()
{
    isGameRunning_ = false;
    SDL_WaitThread(pThreadModelStateUpdater_, NULL);
}

int iat_fb::Controller::theadFuncWrapper(void *data)
{
    Controller *self = static_cast<Controller*>(data);
    return self->threadFunc();
}

int iat_fb::Controller::threadFunc()
{
    while(isGameRunning_)
    {
        SDL_Delay( DELAY);
        if(spModel_->getGameState() == Model::GameState::PLAY)
            spModel_->step();
    }
    return 0;
}
