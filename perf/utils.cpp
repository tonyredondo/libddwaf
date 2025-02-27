// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog
// (https://www.datadoghq.com/). Copyright 2022 Datadog, Inc.

#include <fstream>
#include <iostream>
#include <string>

#include "utils.hpp"

namespace ddwaf::benchmark::utils {

namespace {
// NOLINTNEXTLINE(misc-no-recursion)
void debug_str_helper(std::string &res, const ddwaf_object &p)
{
    if (p.parameterNameLength != 0U) {
        res += p.parameterName;
        res += ": ";
    }
    switch (p.type) {
    case DDWAF_OBJ_INVALID:
        res += "<invalid>";
        break;
    case DDWAF_OBJ_SIGNED:
        res += std::to_string(p.intValue);
        break;
    case DDWAF_OBJ_UNSIGNED:
        res += std::to_string(p.uintValue);
        break;
    case DDWAF_OBJ_STRING:
        res += '"';
        res += std::string_view{p.stringValue, p.nbEntries};
        res += '"';
        break;
    case DDWAF_OBJ_ARRAY:
        res += '[';
        for (decltype(p.nbEntries) i = 0; i < p.nbEntries; i++) {
            debug_str_helper(res, p.array[i]);
            if (i != p.nbEntries - 1) {
                res += ", ";
            }
        }
        res += ']';
        break;
    case DDWAF_OBJ_MAP:
        res += '{';
        for (decltype(p.nbEntries) i = 0; i < p.nbEntries; i++) {
            debug_str_helper(res, p.array[i]);
            if (i != p.nbEntries - 1) {
                res += ", ";
            }
        }
        res += '}';
        break;
    }
}

} // namespace

std::string object_to_string(const ddwaf_object &o) noexcept
{
    std::string res;
    debug_str_helper(res, o);
    return res;
}

// NOLINTNEXTLINE(misc-no-recursion)
ddwaf_object object_dup(const ddwaf_object &o) noexcept
{
    ddwaf_object copy;
    switch (o.type) {
    case DDWAF_OBJ_INVALID:
        ddwaf_object_invalid(&copy);
        break;
    case DDWAF_OBJ_SIGNED:
        ddwaf_object_signed(&copy, o.intValue);
        break;
    case DDWAF_OBJ_UNSIGNED:
        ddwaf_object_unsigned(&copy, o.uintValue);
        break;
    case DDWAF_OBJ_STRING:
        ddwaf_object_stringl(&copy, o.stringValue, o.nbEntries);
        break;
    case DDWAF_OBJ_ARRAY:
        ddwaf_object_array(&copy);
        for (decltype(o.nbEntries) i = 0; i < o.nbEntries; i++) {
            ddwaf_object child_copy = object_dup(o.array[i]);
            ddwaf_object_array_add(&copy, &child_copy);
        }
        break;
    case DDWAF_OBJ_MAP:
        ddwaf_object_map(&copy);
        for (decltype(o.nbEntries) i = 0; i < o.nbEntries; i++) {
            ddwaf_object child_copy = object_dup(o.array[i]);
            ddwaf_object_map_addl(
                &copy, o.array[i].parameterName, o.array[i].parameterNameLength, &child_copy);
        }
        break;
    }
    return copy;
}

std::string read_file(const fs::path &filename)
{
    std::ifstream file(filename.c_str(), std::ios::in);
    if (!file) {
        throw std::system_error(errno, std::generic_category());
    }

    // Create a buffer equal to the file size
    std::string buffer;
    file.seekg(0, std::ios::end);
    buffer.resize(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(&buffer[0], static_cast<int64_t>(buffer.size()));
    file.close();
    return buffer;
}

} // namespace ddwaf::benchmark::utils
