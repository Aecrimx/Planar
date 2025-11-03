#include "renderer.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "configHandle.h"
#include "ftxui/component/loop.hpp"
#include <ftxui/screen/terminal.hpp>
#include <thread>
#include <chrono>

//Include celelalte module
#include "menu_module.h"
#include "dummywindow.h"

using namespace ftxui;

renderer::renderer(menu_module menu_module_, dummywindow dummy)
////////////    initializare module /////////////////
    : main_pages({
          dummy.getWindow(),
          //getToDo etc
      }),
    bar_pages({
        dummy.getWindow()
    }),
      menu_m(std::move(menu_module_)),
      menu(menu_m.getMenu())
{}


void renderer::run() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    auto quit = screen.ExitLoopClosure();

    int* main_selected = menu_m.main_selected_ptr();
    int* bar_selected = menu_m.bar_selected_ptr();

    Component content_main = Container::Tab(main_pages, main_selected);
    Component content_bar = Container::Tab(bar_pages, bar_selected);


    Component layout = Container::Vertical({
        Container::Horizontal({
            menu,
            content_main,
        }),
        content_bar,
    });

    Component app = Renderer(layout, [&] {
        auto dims = ftxui::Terminal::Size();
        int cols = dims.dimx;
        int rows = dims.dimy;
        int menu_width = std::max(20, (cols * 30) / 100); // 30% width cu minim 20 coloane elem
        int bar_height = std::max(3, (rows * 20) / 100);  // 20% height cu min 3 randuri

        auto top = hbox({
            (menu->Render() | vscroll_indicator | frame | borderRounded) | size(WIDTH, EQUAL, menu_width),
            content_main->Render() | borderRounded | flex,
        });

        return vbox({
            top | flex,
            content_bar->Render() | borderRounded | size(HEIGHT, EQUAL, bar_height),
        });
    });

    app = CatchEvent(app, [&](Event event) {
       if (event == Event::Escape) {
           quit();
           return true;
       }
        return false;
    });

    screen.Loop(app);
}


renderer::~renderer() {

}

void renderer::registerMainWidget(ftxui::Component component) {
    main_pages.push_back(std::move(component));
}

void renderer::registerBarWidget(ftxui::Component component) {
    bar_pages.push_back(std::move(component));
}


