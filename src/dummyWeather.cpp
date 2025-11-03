//
// Created by Mircea on 03.11.2025.
//

#include "dummyWeather.h"
#include "APIHandling.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>

#include <sstream>
#include <string>

using namespace ftxui;
using nlohmann::json;

static std::string BuildWeatherText() {
    std::string time_str = "unknown time";
    std::string temp_str = "?";

    try {
        auto a = APIHandling(46, 26); //rly useless dar ca sa taca o bifa
        json j = a.APIHandling::getWeatherJSON();
        if (j.contains("current") && j["current"].is_object()) {
            const auto& cur = j["current"];
            if (cur.contains("time") && cur["time"].is_string()) {
                time_str = cur["time"].get<std::string>();
            }
            if (cur.contains("temperature_2m") && (cur["temperature_2m"].is_number_float() || cur["temperature_2m"].is_number())) {
                double t = cur["temperature_2m"].get<double>();
                std::ostringstream oss; //fancy :)
                oss.setf(std::ios::fixed); // forced fixed-point notation
                oss.precision(1); // one digit only
                oss << t << "°C";
                temp_str = oss.str();
            }
        }
    } catch (...) {
        //defaults rn dar facut sa trimita in schimb eroarea din APIHandling daca apare(acl trimit json objects cu eroare)
    }

    return "Current weather at " + time_str + " in Romania is " + temp_str;
}

dummyWeather::dummyWeather() {
    std::string textW = BuildWeatherText();

    dummyWeather_component = Renderer([textW] {
        return vbox({
            filler(),
            color(Color::Cyan ,text(textW)) | hcenter,
            filler(),
        });
    });
}

ftxui::Component dummyWeather::getdummyWeather() {
    return dummyWeather_component;
}

