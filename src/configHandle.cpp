#include "configHandle.h"
#include <fstream>
#include <iostream>
configHandle::configHandle(std::string appName)
    : appName_(std::move(appName)), configPath_(resolveConfigPath()) {
        load();
}

const json& configHandle::get() const {
    return config_;
}

void configHandle::set(const std::string& key, const json& value) {
    config_[key] = value;
    save();
}

void configHandle::reload() {
    load();
}

fs::path configHandle::path() const {
    return configPath_;
}

fs::path configHandle::resolveConfigPath() const {
#ifdef _WIN32
    const char* appData = std::getenv("APPDATA");
    fs::path baseDir;
    if (appData) {
        baseDir = fs::path(appData);
    } else {
        // fallback to home directory
        const char* homeDir = std::getenv("USERPROFILE");
        if (homeDir) {
            baseDir = fs::path(homeDir) / "AppData" / "Roaming";
        } else {
            baseDir = fs::path("./");
        }
    }
#elif defined(__APPLE__)
    const char* homeDir = std::getenv("HOME");
    fs::path baseDir;
    if (homeDir) {
        baseDir = fs::path(homeDir) / "Library" / "Application Support";
    } else {
        baseDir = fs::path("./");
    }
#elif defined(__linux__)
    const char* homeDir = std::getenv("HOME");
    fs::path baseDir;
    if (homeDir) {
        baseDir = fs::path(homeDir) / ".config";
    } else {
        baseDir = fs::path("./");
    }
#else
    fs::path baseDir = "./"; // fallback pe current working directory
#endif

    fs::path configDir = baseDir / appName_;
    fs::path configFile = configDir / "config.json";

    try {
        if (!fs::exists(configDir)) {
            fs::create_directories(configDir);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not ensure config dir exists: " << e.what() << '\n';
    }
    return configFile;
}

void configHandle::load() {
    if (!fs::exists(configPath_)) {
        createDefault();
    }

    std::ifstream file(configPath_);
    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << configPath_ << '\n';
    }

    try {
        file >> config_;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void configHandle::save() const {
    std::ofstream file(configPath_);
    if (!file.is_open()) {
        std::cerr << "Could not save config file \n";
    }
    file << config_.dump(4);
}

void configHandle::createDefault() {
    json j;
    j["App name"] = appName_;
    j["Version"] = APP_VERSION;
    j["Main_Window"] = 0;
    j["Bar_Window"] = 0;
    config_ = std::move(j);
    save();
}

int configHandle::getMainKey() const {
    const json& temp = configHandle::get();
    auto it = temp.find("Main_Window");
    if (it != temp.end() && it->is_number()) {
        return it->get<int>();
    }
    return 0;
}

int configHandle::getBarKey() const {
    const json& temp = configHandle::get();
    auto it = temp.find("Bar_Window");
    if (it != temp.end() && it->is_number()) {
        return it->get<int>();
    }
    return 0;
}

std::ostream& operator<<(std::ostream& os, const configHandle& config) {
    os << "Loaded config from: " << config.path() << '\n';
    os << "Object dump:\n" << config.get().dump(4) << "\n";

    return os;
}