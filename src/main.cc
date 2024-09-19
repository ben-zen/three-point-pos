// Copyright 2024 Ben Lewis

#include <iostream>
#include <string>
#include <string_view>

#include <fmt/format.h>

enum class menu_items {
    coffee = 1,
    tea,
    bacon,
    eggs,
    breakfast_taters,
    pancakes
};

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

int main(int argc, char **argv) {
    std::cout << "Welcome to the 3 Point Café POS" << std::endl
                << "To place an order, enter one item number at a time, followed by a return each time." << std::endl
                << "To complete your order, press return again." << std::endl
                << "You will receive your order number after that." << std::endl << std::endl;

    std::cout << "Menu:" << std::endl;
    for(auto item : {menu_items::coffee, menu_items::tea, menu_items::bacon, menu_items::eggs, menu_items::breakfast_taters, menu_items::pancakes}) {
        std::cout << fmt::format("{}.\t{}", static_cast<int>(item), item) << std::endl;
    }

    return 0;
}