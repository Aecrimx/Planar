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
    dummyWeather weather = dummyWeather();
    //note to programmer: genuinely please switch to Make functions la urmatoarea versiune

    renderer r = renderer(menu, window, weather);

    r.run();


    //std::cout << config;

    return 0;
}


// linking static in descarcare-api cmakey.yml template trb sa fie acl cv linie legata de static builds
// Crapa la alea de dau hang ppt ca nu am input in tastatura.txt
// macOS 14 GCC 13 o sa crape 100% din cauza ca asa e el de natura. Mai bine il scoti

