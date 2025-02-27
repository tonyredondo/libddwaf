// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include <log.hpp>
#include <object_store.hpp>
#include <vector>

namespace ddwaf {

object_store::object_store(const manifest &m, ddwaf_object_free_fn free_fn)
    : manifest_(m), obj_free_(free_fn)
{
    if (obj_free_ != nullptr) {
        objects_to_free_.reserve(8);
    }
}

object_store::~object_store()
{
    if (obj_free_ == nullptr) {
        return;
    }
    for (auto &obj : objects_to_free_) { obj_free_(&obj); }
}

bool object_store::insert(const ddwaf_object &input)
{
    if (obj_free_ != nullptr) {
        objects_to_free_.emplace_back(input);
    }

    latest_batch_.clear();

    if (input.type != DDWAF_OBJ_MAP) {
        return false;
    }

    std::size_t entries = static_cast<std::size_t>(input.nbEntries);
    if (entries == 0) {
        // Objects with no addresses are considered valid as they are harmless
        return true;
    }

    const ddwaf_object *array = input.array;
    if (array == nullptr) {
        // Since we have established that the size of the map is not 0, a null
        // array constitutes a malformed map.
        return false;
    }

    objects_.reserve(objects_.size() + entries);

    latest_batch_.reserve(entries);

    for (std::size_t i = 0; i < entries; ++i) {
        auto length = static_cast<std::size_t>(array[i].parameterNameLength);
        if (array[i].parameterName == nullptr || length == 0) {
            continue;
        }

        std::string key(array[i].parameterName, length);
        auto [res, target] = manifest_.get_target(key);
        if (!res) {
            continue;
        }

        objects_[target] = &array[i];
        latest_batch_.emplace(target);
    }

    return true;
}

const ddwaf_object *object_store::get_target(manifest::target_type target) const
{
    auto it = objects_.find(manifest::get_root(target));
    return it != objects_.end() ? it->second : nullptr;
}

} // namespace ddwaf
