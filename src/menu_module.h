//
// Created by Mircea on 01.11.2025.
//

#ifndef OOP_MENU_MODULE_H
#define OOP_MENU_MODULE_H
#include <string>
#include <vector>
#include <ftxui/component/component.hpp>
#include "configHandle.h"


class menu_module {
private:
    std::vector<std::string> main_entries;
    std::vector<std::string> bar_entries;

    int menu_main_selected;
    int menu_bar_selected;

    bool menu_main_show;
    bool menu_bar_show;
public:
    menu_module();
    menu_module(const configHandle &config);

    int get_menu_main_selected() const;
    int get_menu_bar_selected() const;

    ftxui::Component getMenu();

    ~menu_module();
};


#endif //OOP_MENU_MODULE_H