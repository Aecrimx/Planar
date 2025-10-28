#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <cstdlib> //getenv
#include <ftxui/screen/color.hpp>

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <sstream>
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace ftxui;

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string AgnosticGetConfigPath() {
#ifdef _WIN32
    char* appdata = std::getenv("APPDATA");
    std::string base = appdata ? appdata : ".";
    return base + "\\Planar\\config.json";
#elif __APPLE__
    char* home = std::getenv("HOME");
    std::string base = home ? home : ".";
    return base + "/Library/Application Support/Planar/config.json";
#else // Linux/Unix
    char* home = std::getenv("HOME");
    std::string base = home ? home : ".";
    return base + "/.config/Planar/config.json";
#endif
}


class Config {
private:
    std::string filename;

public:
    int menu_selection = 0;
    int bar_selection = 0;

    Config(std::string filename) : filename(std::move(filename)) {
        load();
    }

    // Load or create the config file
    void load() {
        if (fs::exists(filename)) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Could not open existing config file. Using defaults.\n";
                return;
            }

            json j;
            try {
                file >> j;
                menu_selection = j.value("menu_selection", 0);
                bar_selection = j.value("bar_selection", 0);
            } catch (const json::parse_error& e) {
                std::cerr << "json : Failed to parse config file: " << e.what() << "\n";
            }
        } else {
            save();
        }
    }

    void save() {
        json j = {
            {"menu_selection", menu_selection},
            {"bar_selection", bar_selection}
            //for now
        };
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create/save config file.\n";
            return;
        }
        file << std::setw(4) << j << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Config& cfg) {
        return os << "Menu selection: " << cfg.menu_selection << '\n'
                  << "Bar selection: " << cfg.bar_selection << '\n';
    }
};

class MenuContentBase : public ComponentBase{
private:
    std::vector<std::string> main_entries = {
        "Calendar",
        "Journal",
        "Timer"
        //To be added
    };
    std::vector<std::string> bar_entries = {
        "Weather",
        "Checklist"
        //To be added
    };
    std::string menu_title;

    int main_menu_selected = 0;
    int bar_menu_selected = 0;
    bool main_menu_active = false;
    bool bar_menu_active = false;

    Component layout;

public:
    MenuContentBase() {
        layout = Container::Vertical({
        Checkbox("Main", &main_menu_active),
        Radiobox(&main_entries, &main_menu_selected) | Maybe(&main_menu_active),
        Checkbox("Bar", &bar_menu_active),
        Radiobox(&bar_entries, &bar_menu_selected) | Maybe(&bar_menu_active),
    });
        Add(layout); // din ComponentBase; il transforma in child component
    }
    MenuContentBase(int main_menu_selected_, int bar_menu_selected_) // Will load from profile
    : main_menu_selected(main_menu_selected_),
    bar_menu_selected(bar_menu_selected_) {
        layout = Container::Vertical({
        Checkbox("Main", &main_menu_active),
        Radiobox(&main_entries, &main_menu_selected) | Maybe(&main_menu_active),
        Checkbox("Bar", &bar_menu_active),
        Radiobox(&bar_entries, &bar_menu_selected) | Maybe(&bar_menu_active),
    });

        Add(layout);
    }

    //GETTER pt Element de render pasat la clasa care renderuieste
    Element Render() {
        return window(text(menu_title) | bold,
            layout->Render() | borderRounded
            );
    }
    int GetMainSelected() const { return main_menu_selected; }
    int GetBarSelected() const { return bar_menu_selected; }


    ~MenuContentBase() override{}
};
ftxui::Component MenuContent(int& main_menu_selected, int& bar_menu_selected) {
    return ftxui::Make<MenuContentBase>(main_menu_selected, bar_menu_selected);
} // CONVERSIE


class App {
private:
    Config current_config;
    ScreenInteractive& screen;

    //AIDI COMPONENTELE DIVERSE
    Component menu;
    std::vector<Component> main_content_components; // vom baga in asta in aceiasi ordine componentelle ca in menu
    std::vector<Component> bar_content_components;

    int main_menu_selected;
    int bar_menu_selected;

    int width = screen.dimx();
    int height = screen.dimy();

    Component root_component; //entire layout

    Component GetCurrentMain() {
        return main_content_components.at(main_menu_selected);
    }
    Component GetCurrentBarComponent() {
        return bar_content_components.at(bar_menu_selected);
    }

public:
    //App(ScreenInteractive& screen_) : screen(screen_) {}
    App(ScreenInteractive& screen_, Config& current_config_) : screen(screen_),
    current_config(current_config_), main_menu_selected(current_config.menu_selection), bar_menu_selected(current_config_.bar_selection){
        menu = MenuContent(main_menu_selected, bar_menu_selected);

        //si celelalte elemente


        // main_content_components = {
        //     //chestii
        // };
        // bar_content_components = {
        //     //chestii
        // };

        main_content_components = {
            Renderer([] { return text("Calendar Placeholder") | border; }),
            Renderer([] { return text("Journal Placeholder") | border; }),
            Renderer([] { return text("Timer Placeholder") | border; }),
        };

        bar_content_components = {
            Renderer([] { return text("Weather Placeholder") | border; }),
            Renderer([] { return text("Checklist Placeholder") | border; }),
        };

        auto content_container = Container::Vertical({
            menu,
            main_content_components[0], // Toate elementele trb sa fie in container
            main_content_components[1], //altfel crapa
            main_content_components[2],
            bar_content_components[0],
            bar_content_components[1],
        });

        root_component = Renderer(Container::Vertical({
    menu,
    main_content_components[0],
    main_content_components[1],
    main_content_components[2],
    bar_content_components[0],
    bar_content_components[1],
}), [&] {
    auto current_main = GetCurrentMain();
    auto current_bar = GetCurrentBarComponent();

    auto top_section = hbox({
        menu->Render() | border | size(WIDTH, EQUAL, 30),
        current_main->Render() | flex,
    }) | flex | size(HEIGHT, EQUAL, 40);

    auto bottom_section = vbox({
        current_bar->Render() | flex,
    }) | flex_grow | vscroll_indicator | frame;

    return vbox({
        top_section,
        bottom_section,
    }) | border;
});
    }

    void Run() {
        screen.Loop(root_component);
        current_config.menu_selection = main_menu_selected;
        current_config.bar_selection = bar_menu_selected;
        current_config.save();
    }
};





int main() {
    auto screen = ScreenInteractive::Fullscreen();

    Config loaded_cfg(AgnosticGetConfigPath());

    App app(screen, loaded_cfg);
    app.Run();

    return 0;
}