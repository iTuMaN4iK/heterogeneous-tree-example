#include "heterogeneous_tree/node.h"
#include "visitor_impl.h"

#include <sstream>
#include <gtest/gtest.h>

using namespace itmn;

TEST(testSerializer, header) {
    std::stringstream ss;
    visitor::impl::serializer_visitor visitor(ss);
    visitor::impl::deserializer deserializer;
    EXPECT_EQ(deserializer(ss), nullptr);
}

struct expected_visitor_int : visitor::visitor_base<int> {
    void operator()(int _value, int _expected) {
        EXPECT_EQ(_value, _expected);
    }
};

TEST(testSerializer, serializeAndDeserializeInt) {
    node_ptr node = make_node(8);
    std::stringstream ss;
    visitor::impl::serializer_visitor serializer_visitor{ss};
    node->visit(serializer_visitor);
    visitor::impl::deserializer deserializer;
    auto expected = deserializer(ss);
    struct expected_visitor_int : visitor::visitor_base<int> {
        void operator()(int _value, int _expected) {
            EXPECT_EQ(_value, _expected);
        }
    };
    expected->visit(expected_visitor_int{}, 8);
}

TEST(testSerializer, serializeAndDeserializeDouble) {
    node_ptr node = make_node(8.15);
    std::stringstream ss;
    visitor::impl::serializer_visitor serializer_visitor{ss};
    node->visit(serializer_visitor);
    visitor::impl::deserializer deserializer;
    auto expected = deserializer(ss);
    struct expected_visitor_int : visitor::visitor_base<double> {
        void operator()(double _value, double _expected) {
            EXPECT_EQ(_value, _expected);
        }
    };
    expected->visit(expected_visitor_int{}, 8.15);
}

TEST(testSerializer, serializeAndDeserializeStdString) {
    node_ptr node = make_node(std::string("std  string"));
    std::stringstream ss;
    visitor::impl::serializer_visitor serializer_visitor{ss};
    node->visit(serializer_visitor);
    visitor::impl::deserializer deserializer;
    auto expected = deserializer(ss);
    struct expected_visitor_int : visitor::visitor_base<std::string> {
        void operator()(std::string _value, std::string _expected) {
            EXPECT_EQ(_value, _expected);
        }
    };
    expected->visit(expected_visitor_int{}, "std  string");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
