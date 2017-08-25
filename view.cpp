#include <algorithm>
#include <stdexcept>
#include <string.h>
#include "view.h"
#include "model.h"
#include "controller.h"
#include "bird.h"
#include "tube.h"
#ifdef DEBUG
#include <iostream>
#endif

iat_fb::View::View(std::shared_ptr<Model> model):
    spModel_{model}, upController_{std::make_unique<Controller>(model)}
{
    pBirdAnimThread_ = SDL_CreateThread(birdAnimaThreadFuncWrapper, NULL, this);
}

iat_fb::View::~View()
{
    SDL_WaitThread(pBirdAnimThread_, NULL);
}


void iat_fb::View::run()
{
    Mix_PlayMusic(resourceManager_.backgroundMusic(), -1);
    ///////////////MAIN LOOP OF THE GAME///////////////////
    while(isRunning_)
    {
        inputPhase();
        drawPhase();
        if(soundEnabled_)
        {
            if(spModel_->getGameState() != Model::GameState::PLAY)
                Mix_PauseMusic();
            else
                Mix_ResumeMusic();
        }
    }
}

void iat_fb::View::updateView()
{
    draw_ = true;
}

void iat_fb::View::collectedPoint()
{
    if(soundEnabled_)
        Mix_PlayChannel(0, resourceManager_.collectPointSound(), 0);
}

void iat_fb::View::gameOver()
{
    if(soundEnabled_)
        Mix_PlayChannel(0, resourceManager_.gameOverSound(), 0);
}

int iat_fb::View::birdAnimaThreadFuncWrapper(void *data)
{
    View *self = static_cast<View*>(data);
    return self->birdAnimThreadFunc();
}

int iat_fb::View::birdAnimThreadFunc()
{
    while(isRunning_)
    {
        SDL_Delay(BIRD_ANIMATION_DELAY_MS);
        if(spModel_->getGameState() == Model::GameState::PLAY)
        {
            if(++birdFrameX_ >= BIRD_SPRITE_NUM_FRAMES_X)
                birdFrameX_ = 0;
        }
        else
        {
            birdFrameX_ = 0;
        }
        draw_ = true;
    }
    return 0;
}

void iat_fb::View::inputPhase()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                upController_->stopProgram();
                isRunning_ = false;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    upController_->birdUp();
                }
                else if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    upController_->birdDown();
                }
                else if(event.button.button == SDL_BUTTON_MIDDLE)
                {
                    upController_->togglePause();
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        upController_->birdUp();
                        break;
                    case SDLK_DOWN:
                        upController_->birdDown();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_n:
                        upController_->startNewGame();
                        break;
                    case SDLK_s:
                        toggleGameSounds();
                        break;
                    case SDLK_SPACE:
                        upController_->togglePause();
                        break;
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        upController_->stopProgram();
                        isRunning_ = false;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

void iat_fb::View::drawPhase() const
{
    //drawing stuff
    if(draw_)
    {
        SDL_SetRenderDrawColor(resourceManager_.renderer(), 0,255,255,255);
        SDL_RenderClear(resourceManager_.renderer());
        drawBird();
        drawTubes();
        drawGameStatus();
        drawScore();
        SDL_RenderPresent(resourceManager_.renderer());
        draw_ = false;
    }
}

void iat_fb::View::drawBird() const
{
    SDL_Rect birdPos{spModel_->getBirdPosX(), spModel_->getBirdPosY(), BIRD_WIDTH, BIRD_HEIGHT};
    SDL_Rect birdRect{birdFrameX_ * BIRD_WIDTH, 0, BIRD_WIDTH, BIRD_HEIGHT};
    SDL_RenderCopyEx(resourceManager_.renderer(), resourceManager_.birdTexture(), &birdRect, &birdPos,
                     spModel_->getBirdAngle(), nullptr, SDL_FLIP_NONE);
}

void iat_fb::View::drawTubes() const
{
    for(const auto &t: spModel_->getTubes()) {
        if(t->isUp())
        {
            SDL_Rect rct{t->x(), t->y(), TUBE_WIDTH, TUBE_TEXTURE_HEIGHT};
            SDL_RenderCopyEx(resourceManager_.renderer(), resourceManager_.tubeTexture(), nullptr,
                             &rct, 0.f, nullptr, SDL_FLIP_VERTICAL);
        }
        else
        {
            SDL_Rect rct{t->x(), t->y(), TUBE_WIDTH, TUBE_TEXTURE_HEIGHT};
            SDL_RenderCopyEx(resourceManager_.renderer(), resourceManager_.tubeTexture(), nullptr,
                             &rct, 0.f, nullptr, SDL_FLIP_NONE);
        }
    }
}

void iat_fb::View::drawGameStatus() const
{
    if(spModel_->getGameState() != Model::GameState::PLAY)
    {
        int w, h;
        SDL_Rect rct;
        SDL_Texture *texture;
        if(spModel_->getGameState() == Model::GameState::GAME_OVER)
            texture = resourceManager_.gameOverTexture();
        else
            texture = resourceManager_.gamePausedTexture();
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rct.x = (WINDOW_WIDTH - w) / 2;
        rct.y = (WINDOW_HEIGHT - h) / 2;
        rct.w = w;
        rct.h = h;
        SDL_RenderCopy(resourceManager_.renderer(), texture, nullptr, &rct);
    }
}

void iat_fb::View::drawScore() const
{
    int w, h;
    SDL_QueryTexture(resourceManager_.gameScoreTexture(), nullptr, nullptr, &w, &h);
    SDL_Rect scoreWordPos{GAME_SCORE_POSITION_X, GAME_SCORE_POSITION_Y, w, h};
    SDL_RenderCopy(resourceManager_.renderer(), resourceManager_.gameScoreTexture(), nullptr, &scoreWordPos);
    int posX{w + GAME_SCORE_POSITION_X};
    std::string score{std::to_string(spModel_->getGameScore())};
    SDL_SetWindowTitle(resourceManager_.window(), (WINDOW_TITLE + std::string(":") + score).c_str());
    for(unsigned short int i {0}; i < score.size(); ++i)
    {
        SDL_Texture *texture = resourceManager_.digitTexture(std::atoi(score.substr(i, 1).c_str()));
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect rct{posX, GAME_SCORE_POSITION_Y, w, h};
        SDL_RenderCopy(resourceManager_.renderer(), texture, nullptr, &rct);
        posX += w + MARGIN_BETWEEN_LETTERS;
    }

}

void iat_fb::View::toggleGameSounds()
{
    if(soundEnabled_)
    {
        soundEnabled_ = false;
        if(Mix_PlayingMusic())
            Mix_PauseMusic();
    }
    else
    {
        soundEnabled_ = true;
        if(!Mix_PlayingMusic())
            Mix_ResumeMusic();
    }
}
