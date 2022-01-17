#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "Item.hpp"

/* Color definitions */
#define RESET "\033[0m"
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"

void print(const std::vector<Item>& todo_items) {
    if (todo_items.size() == 0) {
        std::cout << "No items in todo list" << std::endl;
    } else {
        for (Item i : todo_items) {
            i.print();
        }
    }
}

std::string normalize(std::string s, bool done) {
    if (done == true) {
        std::basic_string<char> result;
        for (unsigned long i = 0; i < s.size(); i++) {
            if (i % 3 == 0) {
                std::string str;
                str += s[i];
                result.append(str);
            }
        }
        return result;
    }
    return s;
}

void add(std::vector<Item>& todo_items, int argc, char** argv) {
    /* Append items to todo_items */
    for (auto i = 2; i < argc; i++) {
        todo_items.push_back(Item(todo_items.size() + 1, argv[i], false));
    }

    /* Serialize */
    std::ofstream fout;
    fout.open("./elements.todo");
    fout << todo_items.size() << std::endl;
    for (const Item& i : todo_items) {
        if (std::find(i.name.begin(), i.name.end(), ' ') != i.name.end()) {
            fout << "\"" << normalize(i.name, i.done) << "\" ";
        } else {
            fout << normalize(i.name, i.done) << " ";
        }
        if (i.done == true) {
            fout << "true" << std::endl;
        } else {
            fout << "false" << std::endl;
        }
    }
    fout.close();
}

bool in(char character, std::string s) {
    for (char c : s) {
        if (c == character) {
            return true;
        }
    }
    return false;
}

bool validate_inputs_are_integers(int argc, char** argv) {
    std::string valid = "0123456789";
    for (auto i = 2; i < argc; i++) {
        for (auto c : std::string(argv[i])) {
            if (!in(c, valid)) {
                return false;
            }
        }
    }
    return true;
}

bool validate_inputs_are_within_range(const std::vector<Item>& todo_items,
                                      int argc, char** argv) {
    for (auto i = 2; i < argc; i++) {
        if ((unsigned long long)std::stoi(std::string(argv[i])) >
            (unsigned long long)todo_items.size()) {
            return false;
        }
    }
    return true;
}

void mark_as_done(std::vector<Item>& todo_items, int argc, char** argv) {
    for (auto i = 2; i < argc; i++) {
        todo_items[std::stoi(std::string(argv[i])) - 1].strikethrough();
        todo_items[std::stoi(std::string(argv[i])) - 1].done = true;
    }

    /* Serialize */
    std::ofstream fout;
    fout.open("./elements.todo");
    fout << todo_items.size() << std::endl;
    for (const Item& i : todo_items) {
        if (std::find(i.name.begin(), i.name.end(), ' ') != i.name.end()) {
            fout << "\"" << normalize(i.name, i.done) << "\" ";
        } else {
            fout << normalize(i.name, i.done) << " ";
        }
        if (i.done == true) {
            fout << "true" << std::endl;
        } else {
            fout << "false" << std::endl;
        }
    }
    fout.close();
}

void mark_as_not_done(std::vector<Item>& todo_items, int argc, char** argv) {
    for (auto i = 2; i < argc; i++) {
        todo_items[std::stoi(std::string(argv[i])) - 1].normalize();
        todo_items[std::stoi(std::string(argv[i])) - 1].done = false;
    }

    /* Serialize */
    std::ofstream fout;
    fout.open("./elements.todo");
    fout << todo_items.size() << std::endl;
    for (const Item& i : todo_items) {
        if (std::find(i.name.begin(), i.name.end(), ' ') != i.name.end()) {
            fout << "\"" << normalize(i.name, i.done) << "\" ";
        } else {
            fout << normalize(i.name, i.done) << " ";
        }
        if (i.done == true) {
            fout << "true" << std::endl;
        } else {
            fout << "false" << std::endl;
        }
    }
    fout.close();
}

void remove(std::vector<Item>& todo_items, int argc, char** argv) {
    std::vector<int> positions;
    for (int i = 2; i < argc; i++) {
        positions.push_back(std::stoi(std::string(argv[i])));
    }

    /* List positions from greatest to least to make deleting easier */
    std::sort(positions.begin(), positions.end());
    std::reverse(positions.begin(), positions.end());

    /* Erase the items */
    for (const auto& p : positions) {
        todo_items.erase(todo_items.begin() + p - 1);
    }

    /* Change the order numbers */
    for (unsigned long long i = 0; i < todo_items.size(); i++) {
        todo_items[i].order_number = i + 1;
    }

    /* Serialize */
    std::ofstream fout;
    fout.open("./elements.todo");
    fout << todo_items.size() << std::endl;
    for (const Item& i : todo_items) {
        fout << normalize(i.name, i.done) << " ";
        if (i.done == true) {
            fout << "true" << std::endl;
        } else {
            fout << "false" << std::endl;
        }
    }
    fout.close();
}

bool all_done(const std::vector<Item>& todo_items) {
    for (const auto& i : todo_items) {
        if (i.done == false) {
            return false;
        }
    }
    return true;
}

void arrange(std::vector<Item>& todo_items) {
    /* count number of done's */
    int dones = 0;
    for (const auto& item : todo_items) {
        if (item.done == true) {
            dones++;
        }
    }

    /* Sort them */
    unsigned long i = 0;
    while (i < todo_items.size()) {
        /* If number of done == 0, break */
        if (dones == 0) {
            break;
        } else {
            /* Get a vector of the tail of the todo_items; the length is the
             * number of items done */
            auto tail = std::vector<Item>(
                todo_items.begin() + (todo_items.size() - dones),
                todo_items.end());

            /* Check if the tail end is all done */
            if (all_done(tail)) {
                break;
            } else {
                /* Check if the current item is done */
                if (todo_items[i].done == true) {
                    Item temp = todo_items[i];
                    todo_items.erase(todo_items.begin() + i);
                    todo_items.push_back(temp);
                } else {
                    i++;
                    continue;
                }
            }
        }
    }

    /* Reorder */
    for (std::vector<Item>::size_type i = 0; i < todo_items.size(); i++) {
        todo_items[i].order_number = i + 1;
    }

    /* Serialize */
    std::ofstream fout;
    fout.open("./elements.todo");
    fout << todo_items.size() << std::endl;
    for (const Item& i : todo_items) {
        fout << normalize(i.name, i.done) << " ";
        if (i.done == true) {
            fout << "true" << std::endl;
        } else {
            fout << "false" << std::endl;
        }
    }
    fout.close();
}

int main(int argc, char** argv) {
    std::vector<Item> todo_items;

    /* Initialize todo with a file */
    if (std::filesystem::exists("./elements.todo")) {
        std::ifstream fin("./elements.todo");
        if (fin.peek() != std::ifstream::traits_type::eof()) {
            /* read elements */
            std::string line;
            bool first_line = true;
            while (std::getline(fin, line)) {
                if (first_line) {
                    first_line = false;
                    continue;
                } else {
                    /* Check if quotes are in the line */
                    if (std::find(line.begin(), line.end(), '"') !=
                        line.end()) {
                        /* Quotes exist */
                        size_t pos_of_second_quote = 0;
                        for (std::basic_string<char>::size_type i = 1;
                             i < line.size(); i++) {
                            if (line[i] == '"') {
                                pos_of_second_quote = i;
                            }
                        }
                        auto name = line.substr(1, pos_of_second_quote - 1);
                        auto done_string =
                            line.substr(pos_of_second_quote + 2, line.size());
                        bool done = (done_string == "true") ? true : false;
                        todo_items.push_back(
                            Item(todo_items.size() + 1, name, done));
                    } else {
                        /* Parse normally */
                        std::string delimeter = " ";
                        std::string name = line.substr(0, line.find(delimeter));
                        std::string done_string =
                            line.substr(line.find(delimeter) + 1, line.size());
                        bool done = (done_string == "true") ? true : false;
                        todo_items.push_back(
                            Item(todo_items.size() + 1, name, done));
                    }
                }
            }
        }
        fin.close();
    } else {
        std::fprintf(stderr, BOLDRED "Todo file doesn't exist\n" RESET);
        std::cout << "Creating todo file...\n" << std::endl;

        std::ofstream fout("./elements.todo");
        fout.close();
    }

    if (argc == 1) {
        /* List out items */
        print(todo_items);
    } else {
        if (std::string(argv[1]) == "add") {
            if (argc <= 2) {
                std::fprintf(
                    stderr, BOLDRED
                    "Usage for todo add requires arguments to add\n" RESET);
            } else {
                /* Add items */
                add(todo_items, argc, argv);
                print(todo_items);
            }
        } else if (std::string(argv[1]) == "done") {
            /* Validate input */
            if (argc > 2) {
                if (validate_inputs_are_integers(argc, argv)) {
                    if (validate_inputs_are_within_range(todo_items, argc,
                                                         argv)) {
                        /* Mark items as done */
                        mark_as_done(todo_items, argc, argv);
                        print(todo_items);

                        /* Serialize */
                    } else {
                        std::fprintf(stderr,
                                     BOLDRED
                                     "All items to mark done must be within "
                                     "range: 1-%lu\n" RESET,
                                     todo_items.size());
                    }
                } else {
                    std::fprintf(
                        stderr, BOLDRED
                        "All items to mark done must me numbers\n" RESET);
                }
            } else {
                std::fprintf(stderr, BOLDRED
                             "Usage for todo done requires arguments to mark "
                             "done\n" RESET);
            }
        } else if (std::string(argv[1]) == "restore") {
            /* Validate input */
            if (argc > 2) {
                if (validate_inputs_are_integers(argc, argv)) {
                    if (validate_inputs_are_within_range(todo_items, argc,
                                                         argv)) {
                        /* Restore items */
                        mark_as_not_done(todo_items, argc, argv);
                        print(todo_items);

                        /* Serialize */
                    } else {
                        std::fprintf(stderr,
                                     BOLDRED
                                     "All items to restore must be within "
                                     "range: 1-%lu\n" RESET,
                                     todo_items.size());
                    }
                } else {
                    std::fprintf(
                        stderr,
                        BOLDRED "All items to restore must me numbers\n" RESET);
                }
            } else {
                std::fprintf(stderr, BOLDRED
                             "Usage for todo restore requires arguments to "
                             "mark done\n" RESET);
            }
        } else if (std::string(argv[1]) == "remove") {
            /* Validate input */
            if (argc > 2) {
                if (validate_inputs_are_integers(argc, argv)) {
                    if (validate_inputs_are_within_range(todo_items, argc,
                                                         argv)) {
                        /* Mark items as done */
                        remove(todo_items, argc, argv);
                        print(todo_items);
                    } else {
                        std::fprintf(stderr,
                                     BOLDRED
                                     "All items to remove must be within "
                                     "range: 1-%lu\n" RESET,
                                     todo_items.size());
                    }
                } else {
                    std::fprintf(stderr, BOLDRED
                                 "All items to remove must me numbers\n" RESET);
                }
            } else {
                std::fprintf(stderr, BOLDRED
                             "Usage for todo remove requires arguments to mark "
                             "done\n" RESET);
            }
        } else if (std::string(argv[1]) == "arrange") {
            if (argc == 2) {
                arrange(todo_items);
                print(todo_items);
            } else {
                std::fprintf(
                    stderr, BOLDRED
                    "arrange doesn't require any additional inputs\n" RESET);
            }
        } else {
            std::fprintf(stderr, BOLDRED "Invalid usage\n" RESET);
            std::fprintf(
                stderr,
                "Usage:\n\ttodo\n\ttodo add [...items...]\n\ttodo done "
                "[...item id's...]\n\ttodo remove [...item id's...]\n\ttodo "
                "arrange\n");
        }
    }
}
