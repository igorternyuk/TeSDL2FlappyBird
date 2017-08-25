#pragma once

namespace iat_fb {
    class ModelListener{
    public:
        virtual void updateView() = 0;
        virtual void collectedPoint() = 0;
        virtual void gameOver() = 0;
    };
}
