//
// Created by Mircea on 03.11.2025.
//

#include "APIHandling.h"
#include <cpr/cpr.h>
//#include <iostream>

using json = nlohmann::json;

APIHandling::APIHandling(double latitude_, double longitude_)
    :latitude(latitude_), longitude(longitude_) {}

json APIHandling::getWeatherJSON() {
    const std::string url = "https://api.open-meteo.com/v1/forecast?latitude=46&longitude=25&current=temperature_2m&timezone=auto";
    //forecast pentru Romania care cere doar temperatura curenta. In viitor voi construi string-ul de la 0 prin aplicatie

    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code != 200) {
        return json{{"error", {
            {"message", "HTTP request failed"},
            {"status_code", r.status_code},
            {"body", r.text}
            }}
        };
    }

    try {
        return json::parse(r.text); // return obiect parsat
    } catch (const std::exception &e) {
        return json{
                {"error", {
                    {"message", "JSON parse error"},
                    {"detail", e.what()}
                }}
        };
    }
}
