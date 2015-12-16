#include "suffix_tree.hpp"

suffix_tree::vertex::vertex():
        parent(nullptr), suffix_link(nullptr), depth(0), parent_edge(), edges() {
}

suffix_tree::vertex::vertex(vertex *parent_,
                            size_t depth_ = 0,
                            std::map<size_t, edge>::iterator parent_edge_ = std::map<size_t, edge>::iterator()):
        parent(parent_), suffix_link(nullptr), depth(depth_), parent_edge(parent_edge_), edges() {
}

suffix_tree::edge::edge(size_t string_begin_, size_t string_end_, vertex *from_, vertex *to_):
        string_begin(string_begin_), string_end(string_end_), from(from_), to(to_) {
}

suffix_tree::position::position(vertex *last_vertex_):
        last_vertex(last_vertex_), current_edge(), edge_position(0) {
}

suffix_tree::suffix_tree(const std::vector<size_t> &string_):
        string(add_0(string_)) {
    check_string();
    build();
}

suffix_tree::~suffix_tree() {
}

std::vector<size_t> &&suffix_tree::add_0(std::vector<size_t> string_) {
    string_.push_back(0);
    return std::move(string_);
}

bool suffix_tree::next_position(position &pos, size_t symbol) {
    if (pos.edge_position == 0) {
        auto next_edge = pos.last_vertex->edges.find(symbol);

        if (next_edge == pos.last_vertex->edges.end()) {
            return false;
        }

        pos.current_edge = next_edge;
        pos.edge_position = 1;
    } else {
        if (string[pos.current_edge->second.string_begin + pos.edge_position] != symbol) {
            return false;
        }

        ++pos.edge_position;
    }

    if (pos.edge_position == pos.current_edge->second.string_end - pos.current_edge->second.string_begin) {
        pos.last_vertex = pos.current_edge->second.to;
        pos.edge_position = 0;
    }

    return true;
}

void suffix_tree::split_edge_in_position(suffix_tree::position &pos) {
    if (pos.edge_position == 0) {
        return;
    }

    vertex *new_vertex =
            new vertex(pos.current_edge->second.from, pos.last_vertex->depth + pos.edge_position, pos.current_edge);
    new_vertex->edges
              .insert(std::make_pair(string[pos.current_edge->second.string_begin + pos.edge_position],
                                     edge(pos.current_edge->second.string_begin + pos.edge_position,
                                          pos.current_edge->second.string_end,
                                          new_vertex,
                                          pos.current_edge->second.to)));

    pos.current_edge->second.to->parent = new_vertex;
    pos.current_edge->second.to->parent_edge = new_vertex->edges.begin();

    pos.current_edge->second.to = new_vertex;
    pos.current_edge->second.string_end = new_vertex->edges.begin()->second.string_begin;

    pos.last_vertex = new_vertex;
    pos.edge_position = 0;
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
    vertex *children = new vertex(root, string.size());

    children->parent_edge = root->edges.insert(std::make_pair(string[0], edge(0, string.size(), root, children))).first;
    children->parent = root;
}

suffix_tree::vertex *suffix_tree::get_alpha_locus(suffix_tree::vertex *head) const {
    if (head == root) {
        return root;
    }

    if (head->depth == 1) {
        return root;
    } else {
        return head->suffix_link;
    }
}

suffix_tree::vertex *suffix_tree::rescanning(suffix_tree::vertex *alpha_locus, size_t beta_begin, size_t beta_end) {
    auto next_edge = alpha_locus->edges.find(string[beta_begin]);

    if (next_edge->second.string_end - next_edge->second.string_begin < beta_end - beta_begin) {
        return rescanning(next_edge->second.to, next_edge->second.string_end, beta_begin);
    } else if (next_edge->second.string_end - next_edge->second.string_begin == beta_end - beta_begin) {
        return next_edge->second.to;
    } else {
        position alpha_beta_locus(alpha_locus);
        alpha_beta_locus.edge_position = beta_end - beta_begin;
        split_edge_in_position(alpha_beta_locus);
        return alpha_beta_locus.last_vertex;
    }
}

suffix_tree::vertex *suffix_tree::scanning(size_t suffix,
                                           suffix_tree::vertex *alpha_beta_locus,
                                           suffix_tree::vertex *head) {
    if (head->suffix_link == nullptr) {
        head->suffix_link = alpha_beta_locus;
    }

    size_t tail_start = string.size() - suffix - head->depth;
    position pos(alpha_beta_locus);
    for (; next_position(pos, string[tail_start]); ++tail_start) { }

    split_edge_in_position(pos);

    vertex *new_vertex = new vertex(pos.last_vertex, pos.last_vertex->depth + string.size() - tail_start);
    pos.last_vertex
       ->edges
       .insert(std::make_pair(string[tail_start], edge(tail_start, string.size(), pos.last_vertex, new_vertex)));

    return pos.last_vertex;
}
