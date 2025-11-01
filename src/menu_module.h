//
// Created by Mircea on 01.11.2025.
//

#ifndef OOP_MENU_MODULE_H
#define OOP_MENU_MODULE_H
#include <string>
#include <vector>
#include <ftxui/component/component.hpp>



class menu_module {
private:
    std::vector<std::string> main_entries;
    std::vector<std::string> bar_entries;

    int menu_main_selected;
    int menu_bar_selected;

    bool menu_main_show;
    bool menu_bar_show;

    ftxui::Component menu_layout; // va fi un Container::Vertical
public:
    menu_module();

    int getMainSelected();
    int getBarSelected();

    ftxui::Component getLayout();

    ~menu_module();
};


#endif //OOP_MENU_MODULE_H