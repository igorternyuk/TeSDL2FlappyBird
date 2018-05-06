#include "model.hpp"
#include "view.hpp"
#include <memory>
#include <stdexcept>
#include <iostream>

using namespace flappyBird;

int main()
{
    try
    {
        auto model{std::make_shared<Model>()};
        auto view{std::make_shared<View>(model)};
        model->addListener(view);
        view->run();
    }
    catch(std::exception &ex){
        std::cout << ex.what() << std::endl;
    }
    return 0;
}
