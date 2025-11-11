//
// Created by Mircea on 03.11.2025.
//

#include "APIHandling.h"
#include <cpr/cpr.h>
#include "configHandle.h"
#include <iostream>

using json = nlohmann::json;

// APIHandling::APIHandling(double latitude_, double longitude_)
//     :latitude(latitude_), longitude(longitude_) {}

APIHandling::APIHandling(configHandle& config)
    :config_(config) {
}


json APIHandling::getWeatherJSON(configHandle& config) {
    std::string location = config.getCachedLocation();

    double latitude, longitude;

    cpr::Response r_geocode = cpr::Get(
        cpr::Url{"https://geocoding-api.open-meteo.com/v1/search"},
        cpr::Parameters{
            {"name", location},
            {"count", "1"},
            {"language", "en"},
            {"format", "json"}
        }
        );

    if (r_geocode.status_code != 200) {
        return json{{"error", {
                {"message", "HTTP request failed - geolocation"},
                {"status_code", r_geocode.status_code},
                {"body", r_geocode.text}
        }}
        };
    }

    try {
        nlohmann::json location_response = nlohmann::json::parse(r_geocode.text);
        latitude = location_response["results"][0]["latitude"];
        longitude = location_response["results"][0]["longitude"];

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }


    //example link:
    //https://api.open-meteo.com/v1/forecast?latitude=44.4323&longitude=26.1063&hourly=temperature_2m,weather_code&timezone=auto&forecast_days=1&timeformat=unixtime

    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.open-meteo.com/v1/forecast"},
        cpr::Parameters{
            {"latitude", std::to_string(latitude)},
            {"longitude", std::to_string(longitude)},
            {"hourly", "temperature_2m,weather_code"},
            {"timezone", "auto"},
            {"forecast_days", "1"},
            {"timeformat", "unixtime"}
        }
    );


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


json APIHandling::getWeatherJSONDummy() {
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


std::ostream &operator<<(std::ostream &os, const APIHandling& handling) {
    //os << "Latitudine: " << latitude << " Longitudine: " << longitude << '\n';
    //will be fully implemented when weather class is fully written
    const json temp = APIHandling::getWeatherJSON(handling.config_);
    os << temp;


    return os;
}