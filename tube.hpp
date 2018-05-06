#pragma once
#include "flappy_bird_constants.hpp"

namespace flappyBird
{
    class Tube
    {
    public:
        explicit Tube(float x, float y, bool isUp);
        inline auto x() const noexcept { return x_; }
        inline auto y() const noexcept { return y_; }
        inline auto isUp() const noexcept { return isUp_; }
        inline auto isOutOfField() const noexcept { return x_ + TUBE_WIDTH < 0; }
        inline void move() { x_ += SPEED; }

    private:
        enum {SPEED = -15};
        float x_, y_;
        bool isUp_;
    };
}

