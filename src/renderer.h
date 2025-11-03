#ifndef OOP_RENDERER_H
#define OOP_RENDERER_H

#include <ftxui/component/component.hpp>
#include <nlohmann/json.hpp>

#include "menu_module.h"
#include "dummywindow.h"
#include "dummyWeather.h"
class renderer {
public:
    renderer(menu_module menu_module_, dummywindow dummy, dummyWeather dummyW); //jank masiv

    void run();

    //Momentan adaugare noi widget-uri / module prin astea direct in main_pages / bar_pages
    void registerMainWidget(ftxui::Component component);
    void registerBarWidget(ftxui::Component component);

    ~renderer();
private:
    std::vector<ftxui::Component> main_pages; // will be init in constructor
    std::vector<ftxui::Component> bar_pages;
    menu_module menu_m; //are ownership pe vizibilitate si indices
    ftxui::Component menu;
};


#endif //OOP_RENDERER_H