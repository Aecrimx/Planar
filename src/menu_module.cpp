//
// Created by Mircea on 01.11.2025.
//

#include "menu_module.h"
#include "configHandle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

menu_module::menu_module(const configHandle &config)
    : main_entries({
          "Calendar",
          "Agenda",
          "Journal",
          "Timer"
          //etc
      }),
    bar_entries({
        "Weather",
        "Checklist"
        //etc
    }),
    menu_main_selected(config.getMainKey()),
    menu_bar_selected(config.getBarKey()),
    menu_main_show(false),
    menu_bar_show(false)
{}


ftxui::Component menu_module::getMenu() {
    auto main_radio = ftxui::Radiobox(&main_entries, &menu_main_selected);
    auto bar_radio  = ftxui::Radiobox(&bar_entries, &menu_bar_selected);

    auto main_indented = ftxui::Renderer(main_radio, [&, main_radio] {
        return ftxui::hbox({ftxui::text("  "), main_radio->Render()});
    });
    auto bar_indented = ftxui::Renderer(bar_radio, [&, bar_radio] {
        return ftxui::hbox({ftxui::text("  "), bar_radio->Render()});
    });

    auto menu = ftxui::Container::Vertical({
        ftxui::Checkbox("Main Window", &menu_main_show),
        main_indented | ftxui::Maybe(&menu_main_show),
        ftxui::Checkbox("Bar Window", &menu_bar_show),
        bar_indented | ftxui::Maybe(&menu_bar_show),
    });

    return menu;
    //Layout-ul pt acest tip de meniu e ft inspirat din exemplul de cod al librariei FTXUI
    //https://github.com/ArthurSonzogni/FTXUI/blob/main/examples/component/maybe.cpp
}

menu_module::menu_module() = default;

menu_module::~menu_module() {
    main_entries.clear();
    bar_entries.clear();
}

int menu_module::get_menu_main_selected() const {
    return menu_main_selected;
}

int menu_module::get_menu_bar_selected() const {
    return menu_bar_selected;
}

int* menu_module::main_selected_ptr() {
    return &menu_main_selected;
}

int* menu_module::bar_selected_ptr() {
    return &menu_bar_selected;
}
