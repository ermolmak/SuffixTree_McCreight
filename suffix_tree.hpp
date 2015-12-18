#pragma once

#include <algorithm>
#include <cstddef>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

class suffix_tree {
public:
    explicit suffix_tree(const std::vector<size_t> &string_);
    ~suffix_tree();
    unsigned long long count_substrings();
private:

    struct edge;

    struct vertex {
        vertex *parent;
        vertex *suffix_link;
        size_t depth;
        size_t tree_num;
        std::map<size_t, edge>::iterator parent_edge;
        std::map<size_t, edge> edges;

        vertex();

        explicit
        vertex(vertex *parent_, size_t tree_num_, size_t depth_, std::map<size_t, edge>::iterator parent_edge_);
    };

    struct edge {
        size_t string_begin, string_end;
        vertex *from, *to;

        edge(size_t string_begin_, size_t string_end_, vertex *from_, vertex *to_);
    };

    struct position {
        vertex *last_vertex;
        std::map<size_t, edge>::iterator current_edge;
        size_t edge_position;

        explicit position(vertex *last_vertex_);
    };

    static std::vector<size_t> &&add_0(std::vector<size_t> string_);
    const std::vector<size_t> numeric_string_;

    vertex *root_;
private:
    bool next_position(position &pos, size_t symbol);

    void split_edge_in_position(size_t suffix_num, position &pos);
    void check_string() const;
    void build();
    void build_first();
    vertex *get_alpha_locus(size_t suffix_num, vertex *head) const;
    vertex *rescanning(size_t suffix_num, vertex *alpha_locus, size_t beta_begin, size_t beta_end);

    vertex *scanning(size_t suffix_num, vertex *alpha_beta_locus, vertex *head);

    void clear(vertex *start);
    unsigned long long count_substrings(vertex *start);
};
