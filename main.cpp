#include <memory>
#include "model.h"
#include "view.h"

using namespace iat_fb;

int main()
{
    auto model{std::make_shared<Model>()};
    auto view{std::make_shared<View>(model)};
    model->addListener(view);
    view->run();
    return 0;
}
