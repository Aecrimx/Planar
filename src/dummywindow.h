//
// Created by Mircea on 01.11.2025.
//

#ifndef OOP_DUMMYWINDOW_H
#define OOP_DUMMYWINDOW_H

#include <ftxui/component/component.hpp>

class dummywindow {
private:
    ftxui::Component dummy_component;
public:
    dummywindow();
    ftxui::Component getWindow();
};

#endif //OOP_DUMMYWINDOW_H