#ifndef OOP_RENDERER_H
#define OOP_RENDERER_H

#include <ftxui/component/component.hpp>
#include <nlohmann/json.hpp>
#include "configHandle.h"
#include "menu_module.h"
#include "dummywindow.h"
class renderer {
public:
    renderer(menu_module menu_module_, dummywindow dummy); //ik, ft janky dar voi pasa aici in constructor TOATE Modulele.

    void run();

    ~renderer();
private:
    std::vector<ftxui::Component> main_pages; // will be init in constructor
    std::vector<ftxui::Component> bar_pages;
    ftxui::Component menu;

    menu_module menu_m;

};


#endif //OOP_RENDERER_H