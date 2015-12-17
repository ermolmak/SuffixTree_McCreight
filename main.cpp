#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "suffix_tree.hpp"

int main() {
    size_t key;
    std::string text;
    std::cin >> key >> text;

    std::vector<size_t> code;
    code.reserve(text.size());

    for (char c : text) {
        code.push_back(static_cast<size_t>(c - 'a' + 1));
    }

    for (size_t i = 0; i < code.size(); ++i) {
        std::vector<size_t> current(code.begin() + i, code.begin() + std::min(code.size(), i + key));
        if (i + key > code.size()) {
            current.insert(current.end(), code.begin(), code.begin() + (i + key - code.size()));
        }

        suffix_tree tree(current);
        std::cout << tree.count_substrings() << ' ';
    }

    return 0;
}