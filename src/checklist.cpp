#include "checklist.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace  ftxui;

checklist::checklist(configHandle &config)
    : config_(config) {
    input_component_ = Input(&input_text_, "Enter new item...");
    loadCheckboxes();
}

void checklist::loadCheckboxes() {
    checkbox_states_.clear();
    checkbox_components_.clear();

    json items = config_.getChecklist();

    if (!items.is_array()) {
        items = json::array();
    }

    size_t idx_counter = 0;
    for (const auto& item : items) {
        bool checked = false;
        std::string text = "";
        try {
            if (item.is_object()) {
                if (item.contains("checked")) {
                    if (item["checked"].is_boolean()) checked = item["checked"].get<bool>();
                    else if (item["checked"].is_number_integer()) checked = item["checked"].get<int>() != 0;
                }
                if (item.contains("text") && item["text"].is_string()) {
                    text = item["text"].get<std::string>();
                }
            }
        } catch (...) {

        }

        auto state = std::make_shared<bool>(checked);
        checkbox_states_.push_back(state);

        auto checkbox = Checkbox(text, state.get());

        const size_t this_idx = idx_counter++;
        checkbox = Renderer(checkbox, [this, this_idx, checkbox] {
            auto element = checkbox->Render();

            if (this_idx < checkbox_states_.size() && *checkbox_states_[this_idx]) { //checked
                element = element | strikethrough | dim;
            }

            // Highlight
            if (static_cast<int>(this_idx) == selected_index_) {
                element = element | inverted | bold;
            }

            return element;
        });

        checkbox = CatchEvent(checkbox, [this, this_idx](Event event) {
            if (event == Event::Return || event == Event::Character(' ')) {
                if (this_idx < checkbox_states_.size()) {
                    syncToConfig();
                }
                return false; // visual toggle handled by the checkbox component already
            }
            return false;
        });

        checkbox_components_.push_back(checkbox);
    }

    if (selected_index_ >= static_cast<int>(checkbox_components_.size())) {
        selected_index_ = std::max(0, static_cast<int>(checkbox_components_.size()) - 1);
    }
}

void checklist::syncToConfig() {
    json items = json::array();

    json current_items = config_.getChecklist();

    const size_t n = checkbox_states_.size();
    for (size_t i = 0; i < n; ++i) {
        json item;
        std::string text;
        if (i < current_items.size() && current_items[i].is_object() && current_items[i].contains("text") && current_items[i]["text"].is_string()) {
            text = current_items[i]["text"].get<std::string>();
        } else {
            text = "";
        }
        bool checked = false;
        if (i < checkbox_states_.size() && checkbox_states_[i]) {
            checked = *checkbox_states_[i];
        }
        item["text"] = text;
        item["checked"] = checked;
        items.push_back(item);
    }

    config_.setChecklist(items);
}

void checklist::addItem(const std::string& text) {
    if (!text.empty()) {
        config_.addChecklistItem(text, false);
        loadCheckboxes();
    }
}

void checklist::removeItem(int index) {
    config_.removeChecklistItem(index);

    auto items = config_.getChecklist();
    if (selected_index_ >= static_cast<int>(items.size()) && selected_index_ > 0) {
        selected_index_--;
    }

    loadCheckboxes();
}

Element checklist::createTitleBar() const {
    return hbox({
        text("Checklist - "),
        text("A") | color(Color::Red) | bold,
        text("dd "),
        text("D") | color(Color::Red) | bold,
        text("elete "),
        text("⎵") | color(Color::Red) | bold,
        text(":Toggle "),
        text("↑↓") | color(Color::Red) | bold,
        text(":Navigate"),
    });
}

ftxui::Component checklist::createAddDialogue() {
    auto add_dialogue_content = Container::Vertical({
        input_component_,
    });

    add_dialogue_content = CatchEvent(add_dialogue_content, [&](Event event) {
        if (event == Event::Escape) {
            show_add_dialogue = false;
            input_text_.clear();
            return true;
        }
        if (event == Event::Return) {
            if (!input_text_.empty()) {
                addItem(input_text_);
                input_text_.clear();
                show_add_dialogue = false;
            }
            return true;
        }
        return false;
    });

    return Renderer(add_dialogue_content, [&] {
        return vbox({
            text("Add New Item") | bold | hcenter,
            separator(),
            input_component_->Render(),
            separator(),
            text("Press Enter to add, Esc to cancel") | dim | hcenter,
        }) | border | center;
    });
}

Component checklist::createDeleteDialogue() {
    auto delete_dialogue_content = Container::Vertical({});

    delete_dialogue_content = CatchEvent(delete_dialogue_content, [&](Event event) {
        if (event == Event::Character('y') || event == Event::Character('Y')) {
            if (!checkbox_components_.empty() && selected_index_ >= 0 &&
                selected_index_ < static_cast<int>(checkbox_components_.size())) {
                removeItem(selected_index_);
            }
            show_delete_dialogue = false;
            return true;
        }
        if (event == Event::Character('n') || event == Event::Character('N') || event == Event::Escape) {
            show_delete_dialogue = false;
            return true;
        }
        return false;
    });

    return Renderer(delete_dialogue_content, [&] {
        std::string item_text = "Unknown";
        json items = config_.getChecklist();

        if (!items.empty() && selected_index_ >= 0 && selected_index_ < static_cast<int>(items.size())) {
            item_text = items[selected_index_]["text"].get<std::string>();
        }

        return vbox({
            text("Delete Item?") | bold | hcenter,
            separator(),
            text("\"" + item_text + "\"") | hcenter,
            separator(),
            text("Press Y to confirm, N or Escape to cancel") | dim | hcenter,
        }) | border | center;
    });
}

Component checklist::createChecklistUI() {
    auto checkbox_container = Container::Vertical({});

    for (auto& checkbox : checkbox_components_) {
        checkbox_container->Add(checkbox);
    }

    return Renderer(checkbox_container, [&] {
        auto title = createTitleBar();

        if (checkbox_components_.empty()) {
            return vbox({
                title | hcenter,
                separator(),
                filler(),
                text("Checklist is empty") | dim | hcenter,
                text("Press 'A' to add an item.") | dim | hcenter,
                filler(),
            });
        }

        Elements item_elements;
        for (auto& checkbox : checkbox_components_) {
            item_elements.push_back(checkbox->Render());
        }

        return vbox({
            title | hcenter,
            separator(),
            vbox(item_elements) | vscroll_indicator | frame | flex,
        });
    });
}

Component checklist::getComponent() {
    auto checklist_ui = createChecklistUI();
    auto add_dialogue = createAddDialogue();
    auto delete_dialogue = createDeleteDialogue();

    auto main_container = Container::Vertical({
        checklist_ui,
    });

    main_container = CatchEvent(main_container, [&](Event event) {
        if (event == Event::ArrowUp) {
            if (selected_index_ > 0) {
                selected_index_--;
            }
            return true;
        }
        if (event == Event::ArrowDown) {
            if (selected_index_ < static_cast<int>(checkbox_components_.size()) - 1) {
                selected_index_++;
            }
            return true;
        }

        if (event == Event::Character(' ')) {
            if (!checkbox_components_.empty() && selected_index_ >= 0 &&
                selected_index_ < static_cast<int>(checkbox_components_.size())) {
                *checkbox_states_[selected_index_] = !*checkbox_states_[selected_index_];
                syncToConfig();
            }
            return true;
        }

        if (event == Event::Character('a') || event == Event::Character('A')) {
            show_add_dialogue = true;
            input_text_.clear();
            return true;
        }

        if (event == Event::Character('d') || event == Event::Character('D')) {
            if (!checkbox_components_.empty()) {
                show_delete_dialogue = true;
            }
            return true;
        }

        return false;
    });

    return Renderer(main_container, [&, add_dialogue, delete_dialogue] {
        auto main_view = main_container->Render();

        if (show_add_dialogue) {
            main_view = dbox({
                main_view | dim,
                add_dialogue->Render() | clear_under | center,
            });
        }

        if (show_delete_dialogue) {
            main_view = dbox({
                main_view | dim,
                delete_dialogue->Render() | clear_under | center,
            });
        }

        return main_view;
    });
}