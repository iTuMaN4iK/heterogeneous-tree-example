#include <heterogeneous_tree/node.h>
#include "visitor_impl/visitor_impl.h"

#include <CLI/CLI.hpp>

#include <sstream>
#include <iostream>

using itmn::node_ptr;
using itmn::make_node;
using itmn::visitor::impl::print_visitor;
using itmn::visitor::impl::serializer_visitor;
using itmn::visitor::impl::deserializer;


node_ptr createTest() {
    node_ptr header = make_node(8);
    auto childOne = header->add_child(make_node(std::string("bar")));
    childOne->add_child(make_node(2.015))->add_child(make_node(9));
    childOne->add_child(make_node(2015));
    childOne->add_child(make_node(std::string("2015")));
    auto childTwo = header->add_child(make_node(std::string("bar 2")));
    childTwo->add_child(make_node(std::string("foo")));
    childTwo->add_child(make_node(6.28318))->add_child(make_node(std::string("hello")));
    return header;
}

void print(node_ptr head, uint32_t offset) {
    if (head == nullptr) {
        std::cerr << "Tree is empty" << std::endl;
    }
    uint32_t tmp = offset;
    while (tmp--) {
        std::cout << "  ";
    }
    head->visit(print_visitor{}, std::cout);
    ++offset;
    for (auto child:head->children()) {
        print(child, offset);
    }
}

void serialize(node_ptr head, serializer_visitor &sv, std::ostream &os) {
    head->visit(sv);
    for (auto child:head->children()) {
        serialize(child, sv, os);
    }
    sv.write_separator();
}

int main(int argc, char **argv) {
    CLI::App app("heterogeneous tree example");

    std::string input{}, output{};
    app.add_option("-i, --input", input, "Path to input file");
    app.add_option("-o, --output", output, "Path to output file");
    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    node_ptr head;
    if (input.empty()) {
        head = createTest();
    } else {
        std::ifstream stream{input, std::ios::binary};
        deserializer ds;
        head = ds(stream);
    }
    print(head, 0);
    if (output.empty()){
        return 0;
    }
    std::ofstream stream{output, std::ios::binary};

    serializer_visitor sv{stream};

    serialize(head, sv, stream);
    return 0;
}
