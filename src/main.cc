// Copyright 2024 Ben Lewis

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

class menu_item {
    const std::vector<menu_item &> get_all();
};

class beverage : menu_item {

    std::string m_name;

    beverage(std::string &&name) : m_name(name) {};
};

 enum class menu_items {
    coffee = 1,
    tea,
    bacon,
    eggs,
    breakfast_taters,
    pancakes
};

std::optional<menu_items> int_to_menu_items(int i) {
    switch (i) {
    case 1:
        return menu_items::coffee;

    case 2:
        return menu_items::tea;

    case 3:
        return menu_items::bacon;

    case 4:
        return menu_items::eggs;

    case 5:
        return menu_items::breakfast_taters;

    case 6:
        return menu_items::pancakes;
    }

    return std::nullopt;
}

template<>
struct fmt::formatter<menu_items> : fmt::formatter<std::string_view> {
    template <typename Context>
    auto format(const menu_items &item, Context &context) const -> auto {
        switch (item) {
        case menu_items::coffee:
            return formatter<std::string_view>::format("Coffee", context);

        case menu_items::tea:
            return formatter<std::string_view>::format("Tea", context);

        case menu_items::bacon:
            return formatter<std::string_view>::format("Bacon", context);

        case menu_items::eggs:
            return formatter<std::string_view>::format("Eggs", context);

        case menu_items::breakfast_taters:
            return formatter<std::string_view>::format("Breakfast Taters", context);

        case menu_items::pancakes:
            return formatter<std::string_view>::format("Pancakes", context);
        }

        // unreachable code
        return context.out();
    } 
};

struct order {
    std::map<menu_items, uint8_t> items;
};

order take_order() {
    order slip{};
    bool done = false;
    std::string input_line{};
    int input_num{};

    std::cout << "Order please:" << std::endl;

    while (!done) {
        std::getline(std::cin, input_line);
        if (input_line.length() == 0) {
            std::cout << "Order collected." << std::endl;
            break;
        }
        input_num = std::stoi(input_line);

        std::optional<menu_items> item_opt = int_to_menu_items(input_num);

        if (!item_opt) {
            std::cout << "No such item: " << input_num << " Try again please." << std::endl;
            continue;
        }

        menu_items item {*item_opt};

        if (slip.items.contains(item)) {
            slip.items[item]++;
        } else {
            slip.items[item] = 1;
        }

        std::cout << fmt::format("{}:\t{}", item, slip.items[item]) << std::endl
                  << "Next item? (Press return to close ticket, Ctrl-C to cancel the ticket.)" << std::endl;
        
    }

    return slip;
}



int main(int argc, char **argv) {
    std::cout << "Welcome to the 3 Point Café POS" << std::endl
                << "To place an order, enter one item number at a time, followed by a return each time." << std::endl
                << "To complete your order, press return again." << std::endl
                << "You will receive your order number after that." << std::endl << std::endl;

    std::cout << "Menu:" << std::endl;
    for(auto item : {menu_items::coffee, menu_items::tea, menu_items::bacon, menu_items::eggs, menu_items::breakfast_taters, menu_items::pancakes}) {
        std::cout << fmt::format("{}.\t{}", static_cast<int>(item), item) << std::endl;
    }

    std::cout << std::endl;

    auto main_menu_buttons = ftxui::Container::Vertical({
        ftxui::Button("Start an order", [](){}, ftxui::ButtonOption::Border()),
        ftxui::Button("Exit", [](){ std::exit(0); }, ftxui::ButtonOption::Border())
    });

    ftxui::Element document =
        ftxui::vbox({
            ftxui::text("The 3-Point Café") | ftxui::flex,
            main_menu_buttons->Render()
        });

    auto screen = ftxui::ScreenInteractive::FitComponent();

    auto component = ftxui::Renderer(main_menu_buttons, [&] {
        return ftxui::vbox({
            ftxui::vbox({
                ftxui::text("The 3-Point Café"),
                ftxui::separator()
            }),
            main_menu_buttons->Render()
        });
    });

    screen.Loop(component);

    order slip = take_order();

    for (auto &line : slip.items) {
        fmt::println("{}:\t{}", line.first, line.second);
    }

    // Send to the order printer, get the order number.

    std::cout << "We got your order, but don't have a numbering system yet." << std::endl;

    return 0;
}
