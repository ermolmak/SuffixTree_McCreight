#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "suffix_tree.hpp"
#include "test.hpp"

// Timus_1706

//int main() {
//    std::ios_base::sync_with_stdio(false);
//
//    size_t key;
//    std::string text;
//    std::cin >> key >> text;
//
//    std::vector<size_t> code;
//    code.reserve(text.size());
//
//    for (char c : text) {
//        code.push_back(static_cast<size_t>(c - 'a' + 1));
//    }
//
//    for (size_t i = 0; i < code.size(); ++i) {
//        std::vector<size_t> current(code.begin() + i, code.begin() + std::min(code.size(), i + key));
//        if (i + key > code.size()) {
//            current.insert(current.end(), code.begin(), code.begin() + (i + key - code.size()));
//        }
//
//        suffix_tree tree(current);
//        std::cout << tree.count_substrings() << ' ';
//    }
//
//    return 0;
//}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// Informatics_111789

//int main() {
//    std::ios_base::sync_with_stdio(false);
//    std::ifstream fin("input.txt");
//    std::ofstream fout("output.txt");
//
//    std::string text;
//    fin >> text;
//
//    std::vector<size_t> code;
//    code.reserve(text.size());
//
//    for (char c : text) {
//        code.push_back(static_cast<size_t>(c - 'a' + 1));
//    }
//
//    fout << suffix_tree(code).count_substrings();
//
//    return 0;
//}