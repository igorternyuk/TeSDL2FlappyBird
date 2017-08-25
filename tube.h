#pragma once
#include "flappy_bird_constants.h"

namespace iat_fb {
    class Tube
    {
    public:
        enum {SPEED = -15};
        explicit Tube(float x, float y, bool isUp);
        inline float x() const noexcept { return x_; }
        inline float y() const noexcept { return y_; }
        inline bool isUp() const noexcept { return isUp_; }
        inline bool isOutOfField() const noexcept { return x_ + TUBE_WIDTH < 0; }
        inline void move() { x_ += SPEED; }

    private:
        float x_, y_;
        bool isUp_;
    };
}

