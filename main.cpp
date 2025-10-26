#include <ftxui/component/component.hpp>       // for Slider, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for separator, operator|, Element, size, text, vbox, xflex, bgcolor, hbox, GREATER_THAN, WIDTH, border, HEIGHT, LESS_THAN
#include <ftxui/screen/color.hpp>  // for Color
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace ftxui;

class WindowBox {
private:
    std::string title_;
    std::string content_; // PT MAI TARZIU ADAUGAT CONTINUTUL WINDOWULUI
public:
    WindowBox(const std::string& title,const std::string& content) : title_(title), content_(content) {} //kinda broken


    Element Render() const {
        Element title = text(title_) | italic; // aici nu merge | hcenter :(
        //TO-DO maybe find a hackish way sa fac titlul aliniat pe centru?
        return window(title, paragraph(content_)); // AICI CONTENTUL
    }
    //TO- DO .inner = funcContent();
    void SetTitle(const std::string& new_title) {title_ = new_title;}
    void SetContent(const std::string& new_content) {content_ = new_content;} //same stuff
};


class BorderRenderer {
private:
    ScreenInteractive& screen;
    std::vector<WindowBox> windows_;
    int width = screen.dimx();
    int height = screen.dimy();
public:
    BorderRenderer(ScreenInteractive& screen_ref) : screen(screen_ref) {

        WindowBox MenuBox = WindowBox("Menu", "Content Menu");
        windows_.push_back(MenuBox);

        WindowBox MainBox = WindowBox("Main", "Content Main");
        windows_.push_back(MainBox);

        WindowBox BarBox = WindowBox("Bar", "Content Bar");
        windows_.push_back(BarBox);
    }

    Element Render() const {
        Element top = hbox({
            windows_[0].Render() | flex,
            windows_[1].Render() | flex,
        }) | flex;

        Element bottom = windows_[2].Render() | flex;

        return vbox({
            top,
            bottom,
        }) | border;
    }
};



class BorderScreen {
private:
    ScreenInteractive screen;
public:
    BorderScreen() : screen(ScreenInteractive::Fullscreen()) {}

    void Run() {
         BorderRenderer renderer(screen);
        auto ui = Renderer([&] {return renderer.Render(); });
        screen.Loop(ui);
    }
};


int main() {
    BorderScreen app;
    app.Run();

    return 0;
}
