#include "visitor_impl.h"
#include <sstream>
#include <gtest/gtest.h>

using namespace itmn;

TEST(testPrint, print_all) {
    node_ptr nodeInt = make_node(8);
    auto doubleNode = nodeInt->add_child(make_node(8.15));
    auto stringNode = doubleNode->add_child(make_node(std::string("std::string")));
    std::stringstream ss;
    nodeInt->visit_all(visitor::impl::print_visitor{}, ss);
    int tmpInt{};
    ss >> tmpInt;
    EXPECT_EQ(tmpInt, 8);
    double tmpDouble{};
    ss >> tmpDouble;
    EXPECT_EQ(tmpDouble, 8.15);
    std::string tmpString{};
    ss >> tmpString;
    EXPECT_EQ(tmpString, "std::string");
}

TEST(testPrint, print_one) {
    node_ptr nodeInt = make_node(8);
    std::stringstream ss;
    nodeInt->visit(visitor::impl::print_visitor{}, ss);
    int tmpInt{};
    ss >> tmpInt;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
