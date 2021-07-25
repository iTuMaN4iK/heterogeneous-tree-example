#pragma once

#include <unordered_map>
#include <string>

namespace itmn {
    const std::string int_str = "int";
    const std::string double_str = "double";
    const std::string std_string_str = "std::string";
    const static std::unordered_map<std::string, uint32_t> s_type_ids_serialization = {{int_str,        1},
                                                                                       {double_str,     2},
                                                                                       {std_string_str, 3}};
}