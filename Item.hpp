#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>

class Item {
    public:
        int order_number;
        std::basic_string<char> name;
        bool done;

    public:
        Item(size_t order_number, std::string name, bool done);

        void strikethrough();
        void normalize();

        void print();

    private:
        std::string from_u8string(const std::u8string& s);
};

#endif
