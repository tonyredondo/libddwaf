// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include <exclusion/rule_filter.hpp>
#include <log.hpp>

namespace ddwaf::exclusion {

rule_filter::rule_filter(
    std::string &&id, std::vector<condition::ptr> &&conditions, std::set<rule::ptr> &&rule_targets)
    : id_(std::move(id)), conditions_(std::move(conditions))
{
    rule_targets_.reserve(rule_targets.size());
    for (auto it = rule_targets.begin(); it != rule_targets.end();) {
        rule_targets_.emplace(std::move(rule_targets.extract(it++).value()));
    }
}
std::unordered_set<rule::ptr> rule_filter::match(const object_store &store,
    const ddwaf::manifest &manifest, cache_type &cache, ddwaf::timer &deadline) const
{
    if (cache.result) {
        return {};
    }

    for (const auto &cond : conditions_) {
        // If there's a (false) cache hit, we only need to run this condition
        // on new parameters.
        bool run_on_new = false;
        auto cached_result = cache.conditions.find(cond);
        if (cached_result != cache.conditions.end()) {
            if (cached_result->second) {
                continue;
            }
            run_on_new = true;
        } else {
            auto [it, res] = cache.conditions.emplace(cond, false);
            cached_result = it;
        }

        // TODO: Condition interface without events
        auto opt_match = cond->match(store, manifest, {}, run_on_new, deadline);
        if (!opt_match.has_value()) {
            cached_result->second = false;
            return {};
        }
        cached_result->second = true;
    }

    cache.result = true;

    return rule_targets_;
}

} // namespace ddwaf::exclusion
