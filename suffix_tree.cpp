#include "suffix_tree.hpp"

suffix_tree::vertex::vertex():
        parent(nullptr), suffix_link(nullptr), depth(0), tree_num(0), parent_edge(), edges() {
}

suffix_tree::vertex::vertex(vertex *parent_,
                            size_t tree_num_ = 0,
                            size_t depth_ = 0,
                            std::map<size_t, edge>::iterator parent_edge_ = std::map<size_t, edge>::iterator()):
        parent(parent_), suffix_link(nullptr), depth(depth_), tree_num(tree_num_), parent_edge(parent_edge_), edges() {
}

suffix_tree::edge::edge(size_t string_begin_, size_t string_end_, vertex *from_, vertex *to_):
        string_begin(string_begin_), string_end(string_end_), from(from_), to(to_) {
}

suffix_tree::position::position(vertex *last_vertex_):
        last_vertex(last_vertex_), current_edge(), edge_position(0) {
}

suffix_tree::suffix_tree(const std::vector<size_t> &string_):
        numeric_string_(add_0(string_)) {
    check_string();
    build();
}

suffix_tree::~suffix_tree() {
    clear(root_);
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
        if (numeric_string_[pos.current_edge->second.string_begin + pos.edge_position] != symbol) {
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

void suffix_tree::split_edge_in_position(size_t suffix_num, suffix_tree::position &pos) {
    if (pos.edge_position == 0) {
        return;
    }

    vertex *new_vertex =
            new vertex(pos.current_edge->second.from,
                       suffix_num,
                       pos.last_vertex->depth + pos.edge_position,
                       pos.current_edge);
    new_vertex->edges
              .insert(std::make_pair(numeric_string_[pos.current_edge->second.string_begin + pos.edge_position],
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
    if (std::any_of(numeric_string_.begin(), numeric_string_.end() - 1, [](size_t x) -> bool { return x == 0; })) {
        throw std::invalid_argument("string contains 0");
    }
}

void suffix_tree::build() {
    build_first();

    vertex *head = root_;
    for (size_t i = 1; i < numeric_string_.size(); ++i) {
        vertex *alpha_locus = get_alpha_locus(i, head);
        size_t beta_size = head->depth - alpha_locus->depth - (head == root_ ? 0 : 1);

        vertex *alpha_beta_locus = rescanning(i, alpha_locus, i - 1 + head->depth - beta_size, i - 1 + head->depth);
        head = scanning(i, alpha_beta_locus, head);
    }
}

void suffix_tree::build_first() {
    root_ = new vertex();
    vertex *children = new vertex(root_, 0, numeric_string_.size());

    children->parent_edge = root_->edges
                                 .insert(std::make_pair(numeric_string_[0],
                                                        edge(0, numeric_string_.size(), root_, children))).first;
    children->parent = root_;
    root_->suffix_link = root_;
}

suffix_tree::vertex *suffix_tree::get_alpha_locus(size_t suffix_num, suffix_tree::vertex *head) const {
    if (head == root_) {
        return root_;
    }

    vertex *contracted_locus = head;
    while (contracted_locus->tree_num >= suffix_num - 1) {
        contracted_locus = contracted_locus->parent;
    }

    if (contracted_locus->depth <= 1) {
        return root_;
    } else {
        return contracted_locus->suffix_link;
    }
}

suffix_tree::vertex *suffix_tree::rescanning(size_t suffix_num,
                                             suffix_tree::vertex *alpha_locus,
                                             size_t beta_begin,
                                             size_t beta_end) {
    if (beta_begin == beta_end) {
        return alpha_locus;
    }

    auto next_edge = alpha_locus->edges.find(numeric_string_[beta_begin]);

    if (next_edge->second.string_end - next_edge->second.string_begin < beta_end - beta_begin) {

        return rescanning(suffix_num,
                          next_edge->second.to,
                          beta_begin + (next_edge->second.string_end - next_edge->second.string_begin),
                          beta_end);
    } else if (next_edge->second.string_end - next_edge->second.string_begin == beta_end - beta_begin) {
        return next_edge->second.to;
    } else {

        position alpha_beta_locus(alpha_locus);
        next_position(alpha_beta_locus, numeric_string_[beta_begin]);        // select right edge
        alpha_beta_locus.edge_position = beta_end - beta_begin;

        split_edge_in_position(suffix_num, alpha_beta_locus);
        return alpha_beta_locus.last_vertex;
    }
}

suffix_tree::vertex *suffix_tree::scanning(size_t suffix_num,
                                           suffix_tree::vertex *alpha_beta_locus,
                                           suffix_tree::vertex *head) {
    if (head->suffix_link == nullptr) {
        head->suffix_link = alpha_beta_locus;
    }

    size_t tail_start = suffix_num - 1 + head->depth + (head == root_ ? 1 : 0);
    position pos(alpha_beta_locus);
    for (; next_position(pos, numeric_string_[tail_start]); ++tail_start) { }

    split_edge_in_position(suffix_num, pos);

    vertex *new_vertex =
            new vertex(pos.last_vertex, suffix_num, pos.last_vertex->depth + numeric_string_.size() - tail_start);
    pos.last_vertex->edges
       .insert(std::make_pair(numeric_string_[tail_start],
                              edge(tail_start, numeric_string_.size(), pos.last_vertex, new_vertex)));

    return pos.last_vertex;
}

void suffix_tree::clear(suffix_tree::vertex *start) {
    for (auto &item : start->edges) {
        clear(item.second.to);
    }

    delete start;
}

unsigned long long suffix_tree::count_substrings() {
    unsigned long long counter = count_substrings(root_);
    counter -= numeric_string_.size();   // 0-end substrings
    return counter;
}

unsigned long long suffix_tree::count_substrings(suffix_tree::vertex *start) {
    unsigned long long counter = 0;
    for (auto &item : start->edges) {
        counter += item.second.to->depth - start->depth + count_substrings(item.second.to);
    }

    return counter;
}
