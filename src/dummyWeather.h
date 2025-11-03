//
// Created by Mircea on 03.11.2025.
//

#ifndef OOP_DUMMYWEATHER_H
#define OOP_DUMMYWEATHER_H

#include <ftxui/component/component.hpp>

class dummyWeather {
public:
    //static ftxui::Component Make(); // Ar fi o idee sa incep sa folosesc functii Make cand ajungem la class Templates insa for now tot gettere prajite
    //Reason? sa nu mai am cod asa de prajit cu constructor cu field privat component si getter
    //https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html

    dummyWeather();
    ftxui::Component getdummyWeather();
private:
    ftxui::Component dummyWeather_component;
};

#endif //OOP_DUMMYWEATHER_H