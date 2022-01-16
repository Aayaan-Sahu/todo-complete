#include "Item.hpp"

Item::Item(size_t order_number, std::string name, bool done_var) {
    this->order_number = order_number;
    this->name = name;
    this->done = false;

    if (done_var == true) {
        this->strikethrough();
    }

    this->done = done_var;
}

std::string Item::from_u8string(const std::u8string& s) {
    return std::string(s.begin(), s.end());
}

void Item::strikethrough() {
    if (this->done == false) {
        std::basic_string<char> result;
        for (auto c : this->name) {
            std::string str;
            str += c;
            std::basic_string<char> x = str;
            std::u8string strike = u8"\u0336";
            result.append(x);
            result.append(this->from_u8string(strike));
        }
        this->name = result;
    }
}

void Item::normalize() {
    if (this->done == true) {
        std::basic_string<char> result;
        for (unsigned long i = 0; i < this->name.size(); i++) {
            if (i % 3 == 0) {
                std::string str;
                str += this->name[i];
                result.append(str);
            }
        }
        this->name = result;
    }
}

void Item::print() {
    std::cout << this->order_number << " " << this->name << std::endl;
}
