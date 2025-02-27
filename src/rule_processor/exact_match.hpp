// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#pragma once

#include <clock.hpp>
#include <rule_processor/base.hpp>
#include <string_view>
#include <unordered_map>
#include <utils.hpp>

namespace ddwaf::rule_processor {

class exact_match : public base {
public:
    using rule_data_type = std::vector<std::pair<std::string_view, uint64_t>>;

    exact_match() = default;
    explicit exact_match(std::vector<std::string> &&data);
    explicit exact_match(const rule_data_type &data);
    ~exact_match() override = default;
    exact_match(const exact_match &) = default;
    exact_match(exact_match &&) = default;
    exact_match &operator=(const exact_match &) = default;
    exact_match &operator=(exact_match &&) = default;

    std::optional<event::match> match(std::string_view str) const override;
    std::string_view name() const override { return "exact_match"; }

protected:
    std::vector<std::string> data_;
    std::unordered_map<std::string_view, uint64_t> values_;
};

} // namespace ddwaf::rule_processor
