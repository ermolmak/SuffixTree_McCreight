#pragma once

#include <algorithm>
#include <cstddef>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

class suffix_tree {
private:
    struct edge;

    struct vertex {
        vertex *parent;
        vertex *suffix_link;
        std::map<size_t, edge> edges;
        std::map<size_t, edge>::iterator parent_edge;

        vertex();
    };

    struct edge {
        size_t first, last;
        vertex *from, *to;


        edge(size_t first, size_t last, vertex *from, vertex *to);
    };

    struct position {
        vertex *last_vertex;
        std::map<size_t, edge>::iterator current_edge;
        size_t edge_position;

        position(const std::map<size_t, edge>::iterator &current_edge);
    };

    static std::vector<size_t> &&add_0(std::vector<size_t> string_);

    const std::vector<size_t> string;
    vertex *root;

public:
    explicit suffix_tree(const std::vector<size_t> &string_);
    ~suffix_tree();
private:
    void check_string() const;
    void build();
    void build_first();
};