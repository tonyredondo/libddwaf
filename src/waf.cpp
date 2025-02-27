// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include <algorithm>
#include <unordered_map>

#include "clock.hpp"
#include <ddwaf.h>
#include <exception.hpp>
#include <log.hpp>
#include <parameter.hpp>
#include <parser/parser.hpp>
#include <stdexcept>
#include <utils.hpp>
#include <waf.hpp>

using namespace std::literals;

namespace ddwaf {

namespace {
obfuscator obfuscator_from_config(const ddwaf_config *config)
{
    std::string_view key_regex;
    std::string_view value_regex;

    if (config != nullptr) {
        if (config->obfuscator.key_regex != nullptr) {
            key_regex = config->obfuscator.key_regex;
        }

        if (config->obfuscator.value_regex != nullptr) {
            value_regex = config->obfuscator.value_regex;
        }
    }

    return obfuscator(key_regex, value_regex);
}

ddwaf::object_limits limits_from_config(const ddwaf_config *config)
{
    ddwaf::object_limits limits;

    if (config != nullptr) {
        if (config->limits.max_container_size != 0) {
            limits.max_container_size = config->limits.max_container_size;
        }

        if (config->limits.max_container_depth != 0) {
            limits.max_container_depth = config->limits.max_container_depth;
        }

        if (config->limits.max_string_length != 0) {
            limits.max_string_length = config->limits.max_string_length;
        }
    }

    return limits;
}

} // namespace

waf::ptr waf::from_config(
    const ddwaf_object &ruleset, const ddwaf_config *config, ddwaf::ruleset_info &info)
{
    try {
        ddwaf::config cfg{
            limits_from_config(config),
            obfuscator_from_config(config),
            config != nullptr ? config->free_fn : ddwaf_object_free,
        };

        ddwaf::ruleset rs;
        parser::parse(ruleset, info, rs, cfg);
        return std::shared_ptr<waf>(new waf(std::move(rs), std::move(cfg)));
    } catch (const std::exception &e) {
        DDWAF_ERROR("%s", e.what());
    }

    return nullptr;
}

void waf::toggle_rules(ddwaf::parameter::map &&input)
{
    for (auto &[key, value] : input) {
        auto it = ruleset_.rules.find(std::string(key));

        if (it == ruleset_.rules.end()) {
            DDWAF_WARN("Attempting to toggle an unknown rule %s", key.data());
            continue;
        }

        it->second->toggle(value);
    }
}

} // namespace ddwaf
