// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.
#pragma once

#include <ddwaf.h>
#include <obfuscator.hpp>
#include <utils.hpp>

namespace ddwaf {

struct object_limits {
    uint32_t max_container_depth{DDWAF_MAX_CONTAINER_DEPTH};
    uint32_t max_container_size{DDWAF_MAX_CONTAINER_SIZE};
    uint32_t max_string_length{DDWAF_MAX_STRING_LENGTH};
};

struct config {
    ddwaf::object_limits limits;
    ddwaf::obfuscator event_obfuscator;
    ddwaf_object_free_fn free_fn{ddwaf_object_free};
};

} // namespace ddwaf
