#pragma once

namespace iat_fb {
    class Bird
    {
    public:
        explicit Bird(float x, float y, float w, float h, float velY_);
        inline float x() const noexcept { return posX_; }
        inline float y() const noexcept { return posY_; }
        inline float angle() const noexcept { return angle_; }
        inline float width() const noexcept { return width_; }
        inline float height() const noexcept { return height_; }
        inline float left() const noexcept { return posX_; }
        inline float right() const noexcept { return posX_ + width_; }
        inline float top() const noexcept { return posY_; }
        inline float bottom() const noexcept { return posY_ + height_; }
        inline float vy() const noexcept { return velY_; }
        void fallDown();
        void pitchUp();
        void dive();
        void reset();
    private:
        static const struct PhysicsConstants
        {
            const float ANGLE_MAX = 45.f;
            const float ANGLE_MIN = -45.f;
            const float FALLLING_DELTA_ANGLE = 0.5f;
            const float DIVE_DELTA_ANGLE = 3.0f;
            const float PITCH_UP_DELTA_ANGLE = -5.0f;
            const float PITCH_UP_ACCELERATION = -1.0f;
            const float DIVE_ACCELERATION = 0.5f;
            const float GRAVITY = 0.1;
        } ph_;

        float posX_, posY_, angle_;
        float width_, height_;
        float velY_;
        void checkFieldLimits();
    };
}

