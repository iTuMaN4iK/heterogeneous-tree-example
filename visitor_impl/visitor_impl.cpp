#include "visitor_impl.h"

#include "types_id.h"

#include <fstream>
#include <iostream>

namespace {
    const std::string separator = "nullptr";

    template<class T>
    void write(std::ostream &os, uint32_t type_id, const T &_value) {
        os.write(reinterpret_cast<const char *>(&type_id), sizeof(type_id));
        os.write(reinterpret_cast<const char *>(&_value), sizeof(_value));
    }

    template<>
    void write(std::ostream &os, uint32_t type_id, const std::string &_value) {
        os.write(reinterpret_cast<const char *>(&type_id), sizeof(type_id));
        const auto length = static_cast<uint32_t>(_value.size());
        os.write(reinterpret_cast<const char *>(&length), sizeof(length));
        if (length > 0)
            os.write(_value.data(), length);
    }

    template<class T>
    void write(std::ostream &os, const T &_value) {
        os.write(reinterpret_cast<const char *>(&_value), sizeof(_value));
    }

    template<>
    void write(std::ostream &os, const std::string &value) {
        const auto length = static_cast<uint32_t>(value.size());
        os.write(reinterpret_cast<const char *>(&length), sizeof(length));
        if (length > 0)
            os.write(value.data(), length);
    }

    template<class T>
    void read(std::istream &_is, T &_value) {
        _is.read(reinterpret_cast<char *>(&_value), sizeof(_value));
    }

    template<>
    void read(std::istream &_is, std::string &_value) {
        uint32_t len{};
        _is.read(reinterpret_cast<char *>(&len), sizeof(len));
        if (len > 0) {
            char *buf = new char[len + 1];
            _is.read(buf, len);
            buf[len] = '\0';
            _value = buf;
            delete[] buf;
        }
    }
}

using namespace itmn::visitor::impl;


serializer_visitor::serializer_visitor(std::ostream &os) : m_os(os) {
    this->write_header();
}

void serializer_visitor::operator()(int _value) {
    auto it = s_type_ids_serialization.find(int_str);
    ::write(m_os, it->second, _value);
}

void serializer_visitor::operator()(double _value) {
    auto it = s_type_ids_serialization.find(double_str);
    ::write(m_os, it->second, _value);
}

void serializer_visitor::operator()(const std::string &_value) {
    auto it = s_type_ids_serialization.find(std_string_str);
    ::write(m_os, it->second, _value);
}

void serializer_visitor::write_separator() {
    auto it = s_type_ids_serialization.find(std_string_str);
    ::write(m_os, it->second, ::separator);
}

void serializer_visitor::write_header() {
    ::write(m_os, static_cast<uint32_t>(s_type_ids_serialization.size()));
    for (auto &[type_name, type_id]:s_type_ids_serialization) {
        ::write(m_os, type_name);
        ::write(m_os, type_id);
    }
}

itmn::node_ptr deserializer::operator()(std::istream &_is) {
    if (!read_header(_is)) {
        std::cerr << "Invalid format file" << std::endl;
        return nullptr;
    }
    return read_node(_is);
}

itmn::node_ptr deserializer::read_node(std::istream &_is) {
    uint32_t type_id{};
    read(_is, type_id);
    itmn::node_ptr node;
    switch (type_id) {
        case 1: {
            int _value{};
            read(_is, _value);
            node = make_node(_value);
            break;
        }
        case 2: {
            double _value{};
            read(_is, _value);
            node = make_node(_value);
            break;

        }
        case 3: {
            std::string _value{};
            read(_is, _value);
            if (_value != ::separator) {
                node = make_node(_value);
            } else {
                return nullptr;
            }
            break;
        }
        default: {
            std::cerr << "Invalid type" << std::endl;
            return nullptr;
        }
    }

    while (_is.peek() != std::ifstream::traits_type::eof()) {
        auto child = read_node(_is);
        if (child == nullptr) {
            return node;
        }
        node->add_child(child);
    }
    return nullptr;
}

bool deserializer::read_header(std::istream &_is) {
    uint32_t count_type{};
    ::read(_is, count_type);
    if (count_type != s_type_ids_serialization.size()) {
        return false;
    }

    for (size_t i = {}; i < count_type; ++i) {
        std::string type_name{};
        ::read(_is, type_name);
        int type_id{};
        ::read(_is, type_id);
        auto iter = s_type_ids_serialization.find(type_name);
        if (iter == s_type_ids_serialization.end() && iter->second != type_id) {
            return false;
        }
    }
    return true;
}

