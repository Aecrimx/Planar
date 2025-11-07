//
// Created by Mircea on 03.11.2025.
//

#ifndef OOP_APIHANDLING_H
#define OOP_APIHANDLING_H

#include <nlohmann/json.hpp>

class APIHandling {
private:
    // double latitude;
    // double longitude;
public:
    //APIHandling(double latitude, double longitude); //future proofing pt ca voi dori
      APIHandling();                                              //sa am o lista pentru a seta tara din aplicatie
                                                    //,deci evident o lista cu latitudini si longitudini

    static nlohmann::json getWeatherJSON();
    friend std::ostream& operator<<(std::ostream& os, const APIHandling& handling);
};


#endif //OOP_APIHANDLING_H