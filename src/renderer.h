#ifndef OOP_RENDERER_H
#define OOP_RENDERER_H

#include <ftxui/component/component.hpp>
#include <nlohmann/json.hpp>
#include <ostream>

#include "menu_module.h"
class renderer {
public:
    renderer(menu_module menu_module_, configHandle& config);

    void run();

    //Momentan adaugare noi widget-uri / module prin astea direct in main_pages / bar_pages
    void registerMainWidget(ftxui::Component component);
    void registerBarWidget(ftxui::Component component);

    ~renderer();
private:
    std::vector<ftxui::Component> main_pages; // will be init in constructor
    std::vector<ftxui::Component> bar_pages;
    menu_module menu_m; //are ownership pe vizibilitate si indices
    ftxui::Component menu;
    configHandle& config_;

    friend std::ostream& operator<<(std::ostream& os, const renderer& r);
};

inline std::ostream& operator<<(std::ostream& os, const renderer& r) {
    os << "renderer{"
       << "main_pages.size=" << r.main_pages.size()
       << ", bar_pages.size=" << r.bar_pages.size()
       << ", menu_main_selected=" << r.menu_m.get_menu_main_selected()
       << ", menu_bar_selected=" << r.menu_m.get_menu_bar_selected()
       << ", has_menu_component=" << static_cast<bool>(r.menu)
       << ", config_path=\"" << r.config_.path().string() << "\"" 
       << "}";
    return os;
}

#endif //OOP_RENDERER_H