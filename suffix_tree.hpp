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
        size_t string_begin, string_end;
        std::map<size_t, edge>::iterator parent_edge;
        std::map<size_t, edge> edges;

        vertex();
        vertex(vertex *parent,
               vertex *suffix_link,
               size_t string_begin,
               size_t string_end,
               std::map<size_t, edge>::iterator parent_edge);
    };

    struct edge {
        size_t string_begin, string_end;
        vertex *from, *to;

        edge(size_t string_begin, size_t string_end, vertex *from, vertex *to);
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
    bool next_position(position &pos, size_t symbol);
    void split_edge_in_position(position &pos);

    void check_string() const;
    void build();
    void build_first();
    vertex *alpha_locus(vertex *head);
};
