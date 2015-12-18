#pragma once

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "suffix_tree.hpp"

#include "gtest/gtest.h"

std::default_random_engine engine(std::random_device{}());

const size_t MIN_SIZE = 1;
const size_t MAX_SIZE = 100;
const size_t MAX_SYMBOL = 10;

std::vector<size_t> generate_string() {
    std::vector<size_t> result;
    size_t size = std::uniform_int_distribution<size_t>(MIN_SIZE, MAX_SIZE)(engine);
    result.reserve(size);

    std::uniform_int_distribution<size_t> distribution(1, MAX_SYMBOL);
    for (size_t i = 0; i < size; ++i) {
        result.push_back(distribution(engine));
    }

    return result;
}

unsigned long long count_trivial(const std::vector<size_t> &string) {
    std::set<std::vector<size_t>> substrings;
    for (auto first = string.begin(); first != string.end(); ++first) {
        for (auto last = first; last != string.end(); ++last) {
            substrings.insert(std::vector<size_t>(first, last + 1));
        }
    }

    return substrings.size();
}

std::string to_string(const std::vector<size_t> &vec) {
    std::ostringstream oss;
    for (size_t i : vec) {
        oss << i << ' ';
    }
    return oss.str();
}

TEST(random_test, random_test) {
    std::vector<size_t> string = generate_string();

    std::string str;
    str += "string: ";
    str += to_string(string);
    SCOPED_TRACE(str);

    auto expected = count_trivial(string);
    size_t actual;
    actual = suffix_tree(string).count_substrings();
    ASSERT_EQ(expected, actual);
}

class TimeTest: public ::testing::Test {
protected:
    std::vector<size_t> string;

    virtual void SetUp() {
        string = generate_string();
    }
};

TEST_F(TimeTest, TimeTest) {
    auto res = suffix_tree(string).count_substrings();
}