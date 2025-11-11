//
// Created by Mircea on 11.11.2025.
//

#ifndef OOP_WEATHER_H
#define OOP_WEATHER_H

#include <ftxui/component/component.hpp>

class weather {
public:
    weather();
    ftxui::Component getWeather();
    friend std::ostream& operator<<(std::ostream& os, const weather& w);
    ~weather();
private:
    ftxui::Component weather_component;

};


#endif //OOP_WEATHER_H