#include "renderer.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "configHandle.h"
#include "ftxui/component/loop.hpp"
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

      menu(menu_module_.getMenu())
{}


void renderer::run() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    auto quit = screen.ExitLoopClosure();

    int main_selected = menu_m.get_menu_main_selected();
    int bar_selected = menu_m.get_menu_bar_selected();


    Component content_main = Container::Tab(main_pages, &main_selected);
    Component content_bar = Container::Tab(bar_pages, &bar_selected);


    Component top_layout = Container::Horizontal({
        menu,
        content_main,
    });

    Component bottom_layout = Container::Vertical({
        content_bar,
    });

    Component layout = Container::Horizontal({
        top_layout,
        bottom_layout,
    });

    Component app = Renderer(layout, [&] {
        return vbox({
            menu->Render() | border | size(WIDTH, EQUAL, 20),
            content_main->Render() | border | flex,
            content_bar->Render() | border | flex
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


