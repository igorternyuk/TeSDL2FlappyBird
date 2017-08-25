#include <algorithm>
#include <time.h>
#include "model.h"
#include "flappy_bird_constants.h"
#ifdef DEBUG
#include <iostream>
#endif

iat_fb::Model::Model():
    bird_{std::make_unique<Bird>(BIRD_X, BIRD_Y, BIRD_WIDTH, BIRD_HEIGHT, BIRD_VELOCITY)}
{
    srand(time(0));
    createTubes();
}

iat_fb::Model::~Model()
{
    tubes_.clear();
}

void iat_fb::Model::startNewGame()
{
    score_ = 0;
    bird_->reset();
    tubes_.clear();
    createTubes();
    gameState_ = GameState::PLAY;
}

void iat_fb::Model::togglePause()
{
    if(gameState_ == GameState::PLAY)
        gameState_ = GameState::PAUSE;
    else if(gameState_ == GameState::PAUSE)
            gameState_ = GameState::PLAY;
}

void iat_fb::Model::pitchUpBird()
{
    if(gameState_ == GameState::PLAY)
    {
        bird_->pitchUp();
        notifyAllListeners();
    }
}

void iat_fb::Model::diveBird()
{
    if(gameState_ == GameState::PLAY)
    {
        bird_->dive();
        notifyAllListeners();
    }
}

void iat_fb::Model::step()
{
    if(bird_->bottom() < WINDOW_HEIGHT)
        bird_->fallDown();
    for(auto &t: tubes_)
        t->move();
    removeTubesOutsideOfTheField();
    if(tubes_.size() < 2 * INIT_TUBE_PAIR_NUMBER)
        addNewTubePair();
    handleBirdTubeCollisions();
    if(hasBirdTouchedGround())
    {
        gameState_ = GameState::GAME_OVER;
    }
    notifyAllListeners();
}

void iat_fb::Model::handleBirdTubeCollisions()
{
    static bool isScoreIncremented {false};
    for(unsigned short int i{0}; i < tubes_.size() / 2; i += 2)
    {
        if(bird_->right() > tubes_.at(i)->x() && bird_->left() < tubes_.at(i)->x() + TUBE_WIDTH)
        {
            if(bird_->y() > tubes_.at(i)->y() + TUBE_TEXTURE_HEIGHT &&
               bird_->bottom() < tubes_.at(i + 1)->y())
            {
                if(!isScoreIncremented)
                {
                    score_ += SCORE_INCREMENT;
                    notifyAllListeners(SignalType::COLLECTED_POINT);
                    isScoreIncremented = true;
                }
                break;
            }
            else
            {
                if(gameState_ == GameState::PLAY)
                    notifyAllListeners(SignalType::GAME_OVER);
                gameState_ = GameState::GAME_OVER;
                break;
            }
        }
        else
        {
            isScoreIncremented = false;
        }
    }
}

void iat_fb::Model::createTubes()
{
    for(int i = 0; i < INIT_TUBE_PAIR_NUMBER; ++i)
    {
        int randY{rand() % TUBE_MAX_HEIGHT};
        tubes_.emplace_back(std::make_unique<Tube>(WINDOW_WIDTH / 2 + i * (TUBE_WIDTH + SPACE_BETWEEN_TUBES_X),
                                                   randY - TUBE_TEXTURE_HEIGHT, true));
        tubes_.emplace_back(std::make_unique<Tube>(WINDOW_WIDTH / 2 + i * (TUBE_WIDTH + SPACE_BETWEEN_TUBES_X),
                                                   randY + SPACE_BETWEEN_TUBES_Y, false));
    }
}

void iat_fb::Model::addNewTubePair()
{
    float lastTubeX {tubes_.at(tubes_.size() - 1)->x()};
    int randY{rand() % TUBE_MAX_HEIGHT};
    tubes_.emplace_back(std::make_unique<Tube>(lastTubeX + TUBE_WIDTH + SPACE_BETWEEN_TUBES_X,
                                               randY - TUBE_TEXTURE_HEIGHT, true));
    tubes_.emplace_back(std::make_unique<Tube>(lastTubeX + TUBE_WIDTH + SPACE_BETWEEN_TUBES_X,
                                               randY + SPACE_BETWEEN_TUBES_Y, false));
}

void iat_fb::Model::removeTubesOutsideOfTheField()
{
    tubes_.erase(std::remove_if(tubes_.begin(), tubes_.end(),
                                [&](const auto &tube) { return tube->isOutOfField(); }), tubes_.end());
}

void iat_fb::Model::addListener(std::shared_ptr<ModelListener> spListener)
{
    listeners_.push_back(spListener);
}

void iat_fb::Model::removeListener(std::shared_ptr<ModelListener> wpListener)
{
    listeners_.remove_if([wpListener](std::weak_ptr<ModelListener> p){
        return !(p.owner_before(wpListener) || wpListener.owner_before(p));
    });
}

void iat_fb::Model::notifyAllListeners(SignalType st)
{
    for(auto l: listeners_)
    {
        //Скот Мейерс, 42 Рекомендации -  стр.143
        auto spListener = l.lock(); //Creates shared pointer from weak pointer
        if(spListener != nullptr)
        {
            switch(st)
            {
                case SignalType::UPDATE_VIEW:
                    spListener->updateView();
                    break;
                case SignalType::COLLECTED_POINT:
                    spListener->collectedPoint();
                    break;
                case SignalType::GAME_OVER:
                    spListener->gameOver();
                    break;
                default:
                    break;
            }
        }
    }
}
