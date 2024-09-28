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

#include "external/args.hxx"

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

int order_narrative() {
    std::cout << "Welcome to the 3 Point Café POS" << std::endl
                << "To place an order, enter one item number at a time, followed by a return each time." << std::endl
                << "To complete your order, press return again." << std::endl
                << "You will receive your order number after that." << std::endl << std::endl;

    std::cout << "Menu:" << std::endl;
    for(auto item : {menu_items::coffee, menu_items::tea, menu_items::bacon, menu_items::eggs, menu_items::breakfast_taters, menu_items::pancakes}) {
        std::cout << fmt::format("{}.\t{}", static_cast<int>(item), item) << std::endl;
    }

    std::cout << std::endl;

    order slip = take_order();

    for (auto &line : slip.items) {
        fmt::println("{}:\t{}", line.first, line.second);
    }

    // Send to the order printer, get the order number.

    std::cout << "We got your order, but don't have a numbering system yet." << std::endl;

    return 0;
}

int order_interactive() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    ftxui::Element current_page{};

    auto main_menu_buttons = ftxui::Container::Vertical({
        ftxui::Button("Start an order", [](){}, ftxui::ButtonOption::Border()),
        ftxui::Button("Exit", [&screen](){ screen.Exit(); }, ftxui::ButtonOption::Border())
    });

    ftxui::Element main_page =
        ftxui::vbox({
            ftxui::text("The 3-Point Café") | ftxui::hcenter | ftxui::bold,
            main_menu_buttons->Render()
    });

    

    auto component = ftxui::Renderer(main_menu_buttons, [&] {
        return current_page;
    });

    current_page = main_page;
    screen.Loop(component);

    return 0;
}

int main(int argc, char **argv) {
    args::ArgumentParser parser{"You, too can run the till at the Three Point Café; put in an order, and join the queue!"};
    args::HelpFlag help{parser, "help", "Display this menu", {'h', "help"}};
    args::Group commands{parser, "commands"};
    args::Command interactive{commands, "interactive", "Run the order kiosk TUI"};
    args::Command console{commands, "console", "Run a single order via a textual interface."};

    try
    {
        parser.ParseCLI(argc, argv);

    }
    catch(args::Help&)
    {
        std::cout << parser << std::endl;
        return 0;
    }
    catch (args::ParseError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError &e)
    {
        // Validate that we're not the shell itself.
        // If argv[0] == $SHELL and argc == 1, we're
        // running as the shell.
        
        const char *shell_env = std::getenv("SHELL");
        if (argc != 1 || shell_env == nullptr || (strcmp(argv[0], shell_env) != 0)) {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return 2;
        }

        interactive.Match();
    }

    if (interactive)
    {
        return order_interactive();
    }
    else if (console)
    {
        return order_narrative();
    }

    std::cout << "hit the end of the options, and the sidewalk..." << std::endl;

    return 0;
}
