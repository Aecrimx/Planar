//
// Created by Mircea on 01.11.2025.
//

#include "dummywindow.h"
#include <ftxui/dom/elements.hpp>
using namespace ftxui;

dummywindow::dummywindow() {
    dummy_component = Renderer([] {
        return window(text("WORK IN PROGRESS"), text(""));
    });
}


dummywindow::dummywindow(const dummywindow& other) : dummy_component(other.dummy_component) {}


dummywindow& dummywindow::operator=(const dummywindow& other) {
    if (this != &other) {
        dummy_component = other.dummy_component;
    }
    return *this;
}

ftxui::Component dummywindow::getWindow() {
    return dummy_component;
}

