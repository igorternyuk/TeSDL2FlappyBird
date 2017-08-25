#include "bird.h"
#include <math.h>
#ifdef DEBUG
#include <iostream>
#endif
#include "flappy_bird_constants.h"

iat_fb::Bird::Bird(float x, float y, float w, float h, float vy):
    posX_{x}, posY_{y}, angle_{0.f}, width_{w}, height_{h}, velY_{vy}
{}

const iat_fb::Bird::PhysicsConstants iat_fb::Bird::ph_;

void iat_fb::Bird::fallDown()
{
    velY_ += ph_.GRAVITY;
    angle_ = angle_ < ph_.ANGLE_MAX ? angle_ + ph_.FALLLING_DELTA_ANGLE : angle_;
    posY_ += velY_;
    checkFieldLimits();
}

void iat_fb::Bird::pitchUp()
{
    velY_ += ph_.PITCH_UP_ACCELERATION;
    angle_ = angle_ > ph_.ANGLE_MIN ? angle_ + ph_.PITCH_UP_DELTA_ANGLE : angle_;
    posY_ += velY_;
    checkFieldLimits();
}

void iat_fb::Bird::dive()
{
    velY_ += ph_.DIVE_ACCELERATION;
    angle_ = angle_ < ph_.ANGLE_MAX ? angle_ + ph_.DIVE_DELTA_ANGLE : angle_;
    posY_ += velY_;
    checkFieldLimits();
}

void iat_fb::Bird::reset()
{
    posX_ = BIRD_X;
    posY_ = BIRD_Y;
    angle_ = 0.f;
    velY_ = BIRD_VELOCITY;
}

void iat_fb::Bird::checkFieldLimits()
{
    if(posY_ < 0)
        posY_ = 0.f;
    else if(bottom() > WINDOW_HEIGHT)
         posY_ = WINDOW_HEIGHT - height_;
}