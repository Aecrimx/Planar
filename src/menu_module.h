//
// Created by Mircea on 01.11.2025.
//

#ifndef OOP_MENU_MODULE_H
#define OOP_MENU_MODULE_H
#include <string>
#include <vector>
#include <ostream>
#include <ftxui/component/component.hpp>
#include "configHandle.h"


class menu_module {
private:
    std::vector<std::string> main_entries;
    std::vector<std::string> bar_entries;

    int menu_main_selected{};
    int menu_bar_selected{};

    bool menu_main_show{false};
    bool menu_bar_show{false};
public:
    menu_module();
    explicit menu_module(const configHandle &config);

    int get_menu_main_selected() const;
    int get_menu_bar_selected() const; // TO DO later sa sterg astea ca le-am inlocuit cu alea de jos


    int* main_selected_ptr();
    int* bar_selected_ptr();

    ftxui::Component getMenu();
    

    ~menu_module();

    friend std::ostream& operator<<(std::ostream& os, const menu_module& m);
};

inline std::ostream& operator<<(std::ostream& os, const menu_module& m) {
    os << "menu_module{"
       << "main_entries.size=" << m.main_entries.size()
       << ", bar_entries.size=" << m.bar_entries.size()
       << ", main_selected=" << m.menu_main_selected
       << ", bar_selected=" << m.menu_bar_selected
       << ", show_main_menu=" << m.menu_main_show
       << ", show_bar_menu=" << m.menu_bar_show
       << "}";
    return os;
}

#endif //OOP_MENU_MODULE_H