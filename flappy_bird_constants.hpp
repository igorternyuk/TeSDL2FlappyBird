#pragma once

#include <string>

namespace flappyBird
{
    static const std::string WINDOW_TITLE {"TeSDL2FlappyBird"};
    enum
    {
        WINDOW_WIDTH = 1200,
        WINDOW_HEIGHT = 600,
        BIRD_WIDTH = 73,
        BIRD_HEIGHT = 58,
        BIRD_VELOCITY = 1,
        TUBE_WIDTH = 256,
        TUBE_TEXTURE_HEIGHT = 720,
        TUBE_MAX_HEIGHT = 300,
        SPACE_BETWEEN_TUBES_Y = 200,
        SPACE_BETWEEN_TUBES_X = 400,
        BIRD_X = WINDOW_WIDTH / 4,
        BIRD_Y = (WINDOW_HEIGHT - BIRD_WIDTH) / 2
    };

}
