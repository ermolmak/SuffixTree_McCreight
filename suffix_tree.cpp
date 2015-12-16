#include "suffix_tree.hpp"

suffix_tree::vertex::vertex(): parent(nullptr), suffix_link(nullptr), edges(), parent_edge() {
}

suffix_tree::edge::edge(size_t first, size_t last, vertex *from, vertex *to):
        first(first), last(last), from(from), to(to) {
}

suffix_tree::position::position(const std::map<size_t, edge>::iterator &current_edge):
        current_edge(current_edge), edge_position(0) {
}

suffix_tree::suffix_tree(const std::vector<size_t> &string_): string(add_0(string_)) {
    check_string();
    build();
}

suffix_tree::~suffix_tree() {
}

std::vector<size_t> &&suffix_tree::add_0(std::vector<size_t> string_) {
    string_.push_back(0);
    return std::move(string_);
}

void suffix_tree::check_string() const {
    if (std::any_of(string.begin(), string.end() - 1, [](size_t x) -> bool { return x == 0; })) {
        throw std::invalid_argument("string contains 0");
    }
}

void suffix_tree::build() {
    build_first();
}

void suffix_tree::build_first() {
    root = new vertex();
    vertex *children = new vertex();

    children->parent_edge = root->edges.insert(std::make_pair(string[0], edge(0, string.size(), root, children))).first;
    children->parent = root;
}
