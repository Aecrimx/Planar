//
// Created by Mircea on 01.11.2025.
//

#ifndef OOP_CONFIGHANDLE_H
#define OOP_CONFIGHANDLE_H
#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

#define APP_VERSION 0.2 // TO DO : merge pt versiuni noi la config file


class configHandle {
private:
    std::string appName_;
    fs::path configPath_;
    json config_;

    fs::path resolveConfigPath() const;
    void load();
    void save() const;
    void createDefault();
public:
    explicit configHandle(std::string appName);
    const json& get() const; //return la config curent

    void set(const std::string& key, const json& value); // update setare + salvare instanta

    void reload();

    int getMainKey() const;
    int getBarKey() const;

    fs::path path() const;

    friend std::ostream& operator<<(std::ostream& os, const configHandle& config);
};


#endif //OOP_CONFIGHANDLE_H