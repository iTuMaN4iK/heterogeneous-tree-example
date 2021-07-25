#pragma once

#include <heterogeneous_tree/visitor.h>
#include <heterogeneous_tree/node.h>

namespace itmn {
    namespace visitor {
        namespace impl {
            struct print_visitor : visitor_base<int, double, std::string> {
                template<class T>
                void operator()(T &_in, std::ostream &os) {
                    os << _in << "\n";
                }
            };

            struct serializer_visitor : visitor_base<int, double, std::string> {
                explicit serializer_visitor(std::ostream &os);

                void operator()(int _value);

                void operator()(double _value);

                void operator()(const std::string &_value);

                void write_separator();

            private:
                void write_header();

            private:
                std::ostream &m_os;
            };

            class deserializer {
            public:
                node_ptr operator()(std::istream &_is);

            private:
                node_ptr read_node(std::istream &_is);

                bool read_header(std::istream &_is);
            };
        }
    }
}