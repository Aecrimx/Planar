//#include <ftxui/component/component.hpp>
//#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
//#include <ftxui/dom/elements.hpp>

#include "./src/renderer.h"
#include "./src/configHandle.h"
#include "./src/menu_module.h"
#include "./src/dummywindow.h"

using namespace ftxui;




int main() {
    configHandle currentConfig("Planar");
    menu_module menu = menu_module(currentConfig);
    dummywindow window = dummywindow();


    renderer r = renderer(menu, window);

    r.run();


    //std::cout << config;

    return 0;
}