#ifndef OOP_RENDERER_H
#define OOP_RENDERER_H

#include <ftxui/component/component.hpp>

class renderer {
public:
    renderer();
    void setComponent(ftxui::Component* component, std::vector<ftxui::Component*>* components);
    /*
     * Momentan ma gandesc ca ar avea sens schema:
     *      Meniul seteaza o optiune -> o pune in config
     *      -> Renderer ia optiunea din config
     *      -> Pentru o optiune, schimba ce e in Component respectiv
     */

    void run();

    ~renderer();
private:
    ftxui::Component menu;
    ftxui::Component main_window;
    ftxui::Component bar_window;
    //as putea sa fac ferestrele pointeri la elemente din vectorul modules
    std::vector<ftxui::Component> modules;
};


#endif //OOP_RENDERER_H