#pragma once

#include "modellistener.hpp"
#include "bird.hpp"
#include "tube.hpp"
#include "flappy_bird_constants.hpp"

#include <vector>
#include <list>
#include <memory>

namespace flappyBird
{
    class Model
    {
    public:
        enum class GameState {PLAY, PAUSE, GAME_OVER};
        explicit Model();
        ~Model();
        //Constant interface
        inline GameState getGameState() const noexcept { return gameState_; }
        inline int getGameScore() const noexcept { return score_; }
        inline auto getBirdPosX() const noexcept { return bird_->x(); }
        inline auto getBirdPosY() const noexcept { return bird_->y(); }
        inline auto getBirdAngle() const noexcept { return bird_->angle(); }
        inline auto getBirdWidth() const noexcept { return bird_->width(); }
        inline auto getBirdHeight() const noexcept { return bird_->height(); }
        inline const std::vector<std::unique_ptr<Tube>>& getTubes() const noexcept { return tubes_; }
        void addListener(std::shared_ptr<ModelListener> spListener);
        void removeListener(std::shared_ptr<ModelListener> wpListener);
        //For controller
        void startNewGame();
        void togglePause();
        void pitchUpBird();
        void diveBird();
        void step();
    private:
        enum
        {
            SCORE_INCREMENT = 5,
            INIT_TUBE_PAIR_NUMBER = 4,
        };
        enum class SignalType
        {
            UPDATE_VIEW,
            COLLECTED_POINT,
            GAME_OVER
        };
        std::unique_ptr<Bird> bird_;
        std::vector<std::unique_ptr<Tube>> tubes_;
        std::list<std::weak_ptr<ModelListener>> listeners_;
        GameState gameState_{GameState::PLAY};
        int score_{0};
        inline bool hasBirdTouchedGround() const noexcept { return bird_->bottom() >= WINDOW_HEIGHT; }
        void handleBirdTubeCollisions();
        void createTubes();
        void addNewTubePair();
        void removeTubesOutsideOfTheField();
        void notifyAllListeners(SignalType st = SignalType::UPDATE_VIEW);
    };
}

