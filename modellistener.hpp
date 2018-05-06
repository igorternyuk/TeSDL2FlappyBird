#pragma once

namespace flappyBird
{
    class ModelListener
    {
    public:
        explicit ModelListener() {}
        virtual ~ModelListener() {}
        virtual void updateView() = 0;
        virtual void collectedPoint() = 0;
        virtual void gameOver() = 0;
    };
}
