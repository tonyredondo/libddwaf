// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include <algorithm>
#include <exception.hpp>
#include <exclusion/object_filter.hpp>
#include <log.hpp>
#include <manifest.hpp>
#include <parameter.hpp>
#include <parser/common.hpp>
#include <rule.hpp>
#include <rule_processor/exact_match.hpp>
#include <rule_processor/ip_match.hpp>
#include <rule_processor/is_sqli.hpp>
#include <rule_processor/is_xss.hpp>
#include <rule_processor/phrase_match.hpp>
#include <rule_processor/regex_match.hpp>
#include <ruleset.hpp>
#include <ruleset_info.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using ddwaf::manifest;
using ddwaf::manifest_builder;
using ddwaf::parameter;
using ddwaf::parser::at;
using ddwaf::rule_processor::base;

namespace ddwaf::parser::v2 {

namespace {

condition::ptr parse_condition(parameter::map &rule, rule_data::dispatcher &dispatcher,
    manifest_builder &mb, ddwaf::config &cfg,
    ddwaf::condition::data_source source = ddwaf::condition::data_source::values,
    std::vector<PW_TRANSFORM_ID> transformers = {})
{
    auto operation = at<std::string_view>(rule, "operator");
    auto params = at<parameter::map>(rule, "parameters");
    bool is_mutable = false;

    parameter::map options;
    std::shared_ptr<base> processor;
    std::optional<std::string> rule_data_id = std::nullopt;
    if (operation == "phrase_match") {
        auto list = at<parameter::vector>(params, "list");

        std::vector<const char *> patterns;
        std::vector<uint32_t> lengths;

        patterns.reserve(list.size());
        lengths.reserve(list.size());

        for (auto &pattern : list) {
            if (pattern.type != DDWAF_OBJ_STRING) {
                throw ddwaf::parsing_error("phrase_match list item not a string");
            }

            patterns.push_back(pattern.stringValue);
            lengths.push_back((uint32_t)pattern.nbEntries);
        }

        processor = std::make_shared<rule_processor::phrase_match>(patterns, lengths);
    } else if (operation == "match_regex") {
        auto regex = at<std::string>(params, "regex");
        options = at<parameter::map>(params, "options", options);

        auto case_sensitive = at<bool>(options, "case_sensitive", false);
        auto min_length = at<int64_t>(options, "min_length", 0);
        if (min_length < 0) {
            throw ddwaf::parsing_error("min_length is a negative number");
        }

        processor =
            std::make_shared<rule_processor::regex_match>(regex, min_length, case_sensitive);
    } else if (operation == "is_xss") {
        processor = std::make_shared<rule_processor::is_xss>();
    } else if (operation == "is_sqli") {
        processor = std::make_shared<rule_processor::is_sqli>();
    } else if (operation == "ip_match") {
        auto it = params.find("list");
        if (it == params.end()) {
            rule_data_id = at<std::string>(params, "data");
            processor = std::make_shared<rule_processor::ip_match>();
            is_mutable = true;
        } else {
            processor = std::make_shared<rule_processor::ip_match>(it->second);
        }
    } else if (operation == "exact_match") {
        auto it = params.find("list");
        if (it == params.end()) {
            rule_data_id = at<std::string>(params, "data");
            processor = std::make_shared<rule_processor::exact_match>();
            is_mutable = true;
        } else {
            processor = std::make_shared<rule_processor::exact_match>(it->second);
        }
    } else {
        throw ddwaf::parsing_error("unknown processor: " + std::string(operation));
    }

    std::vector<manifest::target_type> targets;
    auto inputs = at<parameter::vector>(params, "inputs");
    if (inputs.empty()) {
        throw ddwaf::parsing_error("empty inputs");
    }

    for (parameter::map input : inputs) {
        auto address = at<std::string>(input, "address");
        auto key_paths = at<parameter::vector>(input, "key_path", parameter::vector());

        if (address.empty()) {
            throw ddwaf::parsing_error("empty address");
        }

        std::vector<std::string> kp;
        for (std::string path : key_paths) {
            if (path.empty()) {
                throw ddwaf::parsing_error("empty key_path");
            }

            kp.push_back(std::move(path));
        }

        auto target = mb.insert(address, std::move(kp));
        targets.push_back(target);
    }

    auto cond = std::make_shared<condition>(std::move(targets), std::move(transformers),
        std::move(processor), cfg.limits, source, is_mutable);

    if (rule_data_id.has_value()) {
        if (operation == "ip_match") {
            dispatcher.register_condition<rule_processor::ip_match>(*rule_data_id, cond);
        } else if (operation == "exact_match") {
            dispatcher.register_condition<rule_processor::exact_match>(*rule_data_id, cond);
        }
    }

    return cond;
}

void parse_rule(parameter::map &rule, ddwaf::ruleset_info &info, manifest_builder &mb,
    ddwaf::ruleset &rs, ddwaf::config &cfg)
{
    auto id = at<std::string>(rule, "id");
    if (rs.rules.find(id) != rs.rules.end()) {
        DDWAF_WARN("duplicate rule %s", id.c_str());
        info.insert_error(id, "duplicate rule");
        return;
    }

    try {
        std::vector<PW_TRANSFORM_ID> rule_transformers;
        auto source = ddwaf::condition::data_source::values;
        auto transformers = at<parameter::vector>(rule, "transformers", parameter::vector());
        for (std::string_view transformer : transformers) {
            PW_TRANSFORM_ID transform_id = PWTransformer::getIDForString(transformer);
            if (transform_id == PWT_INVALID) {
                throw ddwaf::parsing_error("invalid transformer " + std::string(transformer));
            }

            if (transform_id == PWT_KEYS_ONLY) {
                if (!rule_transformers.empty()) {
                    DDWAF_WARN("keys_only transformer should be the first one "
                               "in the list, all transformers will be applied to "
                               "keys and not values");
                }
                source = ddwaf::condition::data_source::keys;
            } else {
                rule_transformers.push_back(transform_id);
            }
        }

        std::vector<condition::ptr> conditions;
        auto conditions_array = at<parameter::vector>(rule, "conditions");
        conditions.reserve(conditions_array.size());

        for (parameter::map cond : conditions_array) {
            conditions.push_back(
                parse_condition(cond, rs.dispatcher, mb, cfg, source, rule_transformers));
        }

        auto tags = at<parameter::map>(rule, "tags");
        auto rule_ptr = std::make_shared<ddwaf::rule>(std::string(id),
            at<std::string>(rule, "name"), at<std::string>(tags, "type"),
            at<std::string>(tags, "category", ""), std::move(conditions),
            at<std::vector<std::string>>(rule, "on_match", {}), at<bool>(rule, "enabled", true));

        rs.insert_rule(rule_ptr);
        info.add_loaded();
    } catch (const std::exception &e) {
        DDWAF_WARN("failed to parse rule '%s': %s", id.c_str(), e.what());
        info.insert_error(id, e.what());
    }
}

std::set<rule::ptr> parse_rules_target(parameter::map &target, ddwaf::ruleset &rs)
{
    auto rule_id = at<std::string>(target, "rule_id", {});
    if (!rule_id.empty()) {
        const auto &rule_it = rs.rules.find(rule_id);
        if (rule_it != rs.rules.end()) {
            return {rule_it->second};
        }
        return {};
    }

    auto tags = at<parameter::map>(target, "tags", {});
    if (tags.empty()) {
        throw ddwaf::parsing_error("empty rules_target tags");
    }

    std::string type;
    std::string category;
    for (auto &[tag, value] : tags) {
        if (tag == "type") {
            type = std::string(value);
        } else if (tag == "category") {
            category = std::string(value);
        } else {
            DDWAF_WARN("Unknown tag %s in rules_target", tag.data());
        }
    }

    if (!type.empty()) {
        if (!category.empty()) {
            return rs.get_rules_by_type_and_category(type, category);
        }
        return rs.get_rules_by_type(type);
    }

    if (!category.empty()) {
        return rs.get_rules_by_category(category);
    }

    throw ddwaf::parsing_error("no supported tags in rules_target");
}

void parse_exclusion_filter(
    parameter::map &filter, manifest_builder &mb, ddwaf::ruleset &rs, ddwaf::config &cfg)
{
    auto id = at<std::string>(filter, "id");
    if (rs.rule_filters.find(id) != rs.rule_filters.end() ||
        rs.input_filters.find(id) != rs.input_filters.end()) {
        DDWAF_WARN("duplicate exclusion filter %s", id.c_str());
        return;
    }

    try {
        // Check for conditions first
        std::vector<condition::ptr> conditions;
        auto conditions_array = at<parameter::vector>(filter, "conditions", {});
        if (!conditions_array.empty()) {
            conditions.reserve(conditions_array.size());

            for (parameter::map cond : conditions_array) {
                conditions.push_back(parse_condition(cond, rs.dispatcher, mb, cfg));
            }
        }

        std::set<rule::ptr> rules_target;
        auto rules_target_array = at<parameter::vector>(filter, "rules_target", {});
        if (rules_target_array.empty()) {
            for (const auto &[id, rule] : rs.rules) { rules_target.emplace(rule); }
        } else {
            for (parameter::map target : rules_target_array) {
                auto rules_subset = parse_rules_target(target, rs);
                rules_target.merge(rules_subset);
            }
        }

        if (filter.find("inputs") == filter.end()) {
            // Rule filter
            if (conditions.empty() && rules_target.empty()) {
                throw ddwaf::parsing_error("empty exclusion filter");
            }

            auto filter_ptr = std::make_shared<exclusion::rule_filter>(
                std::move(id), std::move(conditions), std::move(rules_target));
            rs.rule_filters.emplace(filter_ptr->get_id(), filter_ptr);
        } else {
            // Input filter
            std::unordered_set<manifest::target_type> input_targets;
            exclusion::object_filter obj_filter{cfg.limits};
            auto inputs_array = at<parameter::vector>(filter, "inputs");
            for (parameter::map input_map : inputs_array) {
                auto address = at<std::string>(input_map, "address");

                auto optional_target = mb.find(address);
                if (!optional_target.has_value()) {
                    // This address isn't used by any rule so we skip it.
                    throw ddwaf::parsing_error(
                        "Address " + address + " not used by any existing rule");
                }

                auto key_path = at<std::vector<std::string_view>>(input_map, "key_path", {});

                obj_filter.insert(*optional_target, key_path);
            }

            auto filter_ptr = std::make_shared<exclusion::input_filter>(std::move(id),
                std::move(conditions), std::move(rules_target), std::move(obj_filter));
            rs.input_filters.emplace(filter_ptr->get_id(), filter_ptr);
        }
    } catch (const std::exception &e) {
        DDWAF_WARN("failed to parse filter '%s': %s", id.c_str(), e.what());
    }
}

} // namespace

void parse(parameter::map &ruleset, ruleset_info &info, ddwaf::ruleset &rs, ddwaf::config &cfg)
{
    auto metadata = at<parameter::map>(ruleset, "metadata", {});
    auto rules_version = metadata.find("rules_version");
    if (rules_version != metadata.end()) {
        info.set_version(rules_version->second);
    }

    auto rules_array = at<parameter::vector>(ruleset, "rules");
    rs.rules.reserve(rules_array.size());

    ddwaf::manifest_builder mb;
    for (parameter::map rule : rules_array) {
        try {
            parse_rule(rule, info, mb, rs, cfg);
        } catch (const std::exception &e) {
            DDWAF_WARN("%s", e.what());
            info.add_failed();
        }
    }

    if (rs.rules.empty()) {
        throw ddwaf::parsing_error("no valid rules found");
    }

    auto filters_array = at<parameter::vector>(ruleset, "exclusions", {});
    for (parameter::map filter : filters_array) {
        try {
            parse_exclusion_filter(filter, mb, rs, cfg);
        } catch (const std::exception &e) {
            DDWAF_WARN("%s", e.what());
            info.add_failed();
        }
    }

    rs.manifest = mb.build_manifest();

    auto data_array = at<parameter::vector>(ruleset, "rules_data", {});
    if (!data_array.empty()) {
        rs.dispatcher.dispatch(data_array);
    }

    DDWAF_DEBUG("Loaded %zu rules out of %zu available in the ruleset", rs.rules.size(),
        rules_array.size());
}

} // namespace ddwaf::parser::v2
