#ifndef FLAPPY_BIRD_CONSTANTS_H
#define FLAPPY_BIRD_CONSTANTS_H
#include <string>
namespace iat_fb {
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
    };
    static const int BIRD_X = WINDOW_WIDTH / 4;
    static const int BIRD_Y = (WINDOW_HEIGHT - BIRD_WIDTH) / 2;
    static const int WINDOW_X = (1366 - WINDOW_WIDTH) / 2;
    static const int WINDOW_Y = (768 - WINDOW_HEIGHT) / 2;
}
#endif // FLAPPY_BIRD_CONSTANTS_H
