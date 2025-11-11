// //
// // Created by Mircea on 06.11.2025.
// //
//
#ifndef OOP_CHECKLIST_H
#define OOP_CHECKLIST_H
//
// #include <ftxui/component/component.hpp>
// #include <string>
// #include <vector>
// #include <memory>
// #include <ostream>
// #include "configHandle.h"
//
// struct ChecklistItem {
//     std::string text;
//     bool checked;
// };
//
// inline std::ostream& operator<<(std::ostream& os, const ChecklistItem& item) {
//     os << "ChecklistItem{" << (item.checked ? "[x] " : "[ ] ") << item.text << "}";
//     return os;
// }
//
// class checklist {
// private:
//     configHandle& config_;
//     int selected_index_ = 0;
//
//     bool show_add_dialogue = false;
//     bool show_delete_dialogue = false;
//     std::string input_text_;
//
//     ftxui::Component input_component_;
//     std::vector<std::shared_ptr<bool>> checkbox_states_;
//     std::vector<ftxui::Component> checkbox_components_;
//
//     void loadCheckboxes();
//     void syncToConfig() const;
//     void addItem(const std::string& text);
//     void removeItem(int index);
//     ftxui::Component createChecklistUI() const;
//     ftxui::Component createAddDialogue();
//     ftxui::Component createDeleteDialogue();
//
//     static ftxui::Element createTitleBar();
//
// public:
//     explicit checklist(configHandle& config_);
//     //ftxui::Component getComponent();
//
//     friend std::ostream& operator<<(std::ostream& os, const checklist& c);
// };
//
// inline std::ostream& operator<<(std::ostream& os, const checklist& c) {
//     os << "checklist{"
//        << "selected_index=" << c.selected_index_
//        << ", show_add_dialogue=" << c.show_add_dialogue
//        << ", show_delete_dialogue=" << c.show_delete_dialogue
//        << ", input_text_size=" << c.input_text_.size()
//        << ", checkbox_count=" << c.checkbox_components_.size()
//        << ", has_input_component=" << static_cast<bool>(c.input_component_)
//        << ", config_path=\"" << c.config_.path().string() << "\""
//        << "}";
//     return os;
// }
//
#endif //OOP_CHECKLIST_H