// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include "test.h"

void populateManifest(ddwaf::manifest &manifest)
{
    ddwaf::manifest_builder mb;
    for (auto key : {"value", "key", "mixed", "mixed2"}) { mb.insert(key, {}); }
    manifest = mb.build_manifest();
}

TEST(TestAdditive, TestMultiCall)
{
    // Initialize a PowerWAF rule
    auto rule = readRule(
        R"({version: '2.1', rules: [{id: 1, name: rule1, tags: {type: flow1, category: category1}, conditions: [{operator: match_regex, parameters: {inputs: [{address: arg1}], regex: .*}}, {operator: match_regex, parameters: {inputs: [{address: arg2}], regex: .*}}]}]})");
    ASSERT_TRUE(rule.type != DDWAF_OBJ_INVALID);

    ddwaf_handle handle = ddwaf_init(&rule, nullptr, nullptr);
    ASSERT_NE(handle, nullptr);
    ddwaf_object_free(&rule);

    ddwaf_context context = ddwaf_context_init(handle);
    ASSERT_NE(context, nullptr);

    ddwaf_object param1, param2, tmp;
    ddwaf_object_map(&param1);
    ddwaf_object_map(&param2);

    ddwaf_object_map_add(&param1, "arg1", ddwaf_object_string(&tmp, "string 1"));
    ddwaf_object_map_add(&param2, "arg2", ddwaf_object_string(&tmp, "string 2"));

    ddwaf_result ret;

    // Run with just arg1
    auto code = ddwaf_run(context, &param1, &ret, LONG_TIME);
    EXPECT_EQ(code, DDWAF_OK);
    EXPECT_FALSE(ret.timeout);
    ddwaf_result_free(&ret);

    // Run with both arg1 and arg2
    code = ddwaf_run(context, &param2, &ret, LONG_TIME);
    EXPECT_EQ(code, DDWAF_MATCH);
    EXPECT_FALSE(ret.timeout);
    EXPECT_STREQ(ret.data,
        R"([{"rule":{"id":"1","name":"rule1","tags":{"type":"flow1","category":"category1"}},"rule_matches":[{"operator":"match_regex","operator_value":".*","parameters":[{"address":"arg1","key_path":[],"value":"string 1","highlight":["string 1"]}]},{"operator":"match_regex","operator_value":".*","parameters":[{"address":"arg2","key_path":[],"value":"string 2","highlight":["string 2"]}]}]}])");
    ddwaf_result_free(&ret);

    ddwaf_context_destroy(context);
    ddwaf_destroy(handle);
}

TEST(TestAdditive, TestBad)
{
    ddwaf_result ret;
    EXPECT_EQ(ddwaf_context_init(nullptr), nullptr);

    ddwaf_object object, tmp;
    ddwaf_object_string(&object, "stringvalue");

    // Since the call was performed with a null context, the parameters will not
    // be freed.
    EXPECT_EQ(ddwaf_run(nullptr, &object, &ret, LONG_TIME), DDWAF_ERR_INVALID_ARGUMENT);
    EXPECT_FALSE(ret.timeout);
    ddwaf_object_free(&object);

    auto rule = readRule(
        R"({version: '2.1', rules: [{id: 1, name: rule1, tags: {type: flow1, category: category1}, conditions: [{operator: match_regex, parameters: {inputs: [{address: arg1}], regex: .*}}, {operator: match_regex, parameters: {inputs: [{address: arg2}], regex: .*}}]}]})");
    ASSERT_TRUE(rule.type != DDWAF_OBJ_INVALID);

    ddwaf_handle handle = ddwaf_init(&rule, nullptr, nullptr);
    ASSERT_NE(handle, nullptr);
    ddwaf_object_free(&rule);

    ddwaf_context context = ddwaf_context_init(handle);
    ASSERT_NE(context, nullptr);

    // In case of an invalid object, the parameters will be freed on the spot
    ddwaf_object_string(&object, "stringvalue");
    EXPECT_EQ(ddwaf_run(context, &object, &ret, LONG_TIME), DDWAF_ERR_INVALID_OBJECT);
    EXPECT_FALSE(ret.timeout);

    // In case of timeout, the parameters will be owned by the context and freed
    // during destruction
    object = DDWAF_OBJECT_MAP;
    ddwaf_object_map_add(&object, "arg1", ddwaf_object_string(&tmp, "value"));
    EXPECT_EQ(ddwaf_run(context, &object, &ret, 0), DDWAF_OK);
    EXPECT_TRUE(ret.timeout);
    ddwaf_context_destroy(context);
    ddwaf_destroy(handle);

    EXPECT_NO_FATAL_FAILURE(ddwaf_context_destroy(nullptr));
}

TEST(TestAdditive, TestParameterOverride)
{
    // Initialize a PowerWAF rule
    auto rule = readRule(
        R"({version: '2.1', rules: [{id: 1, name: rule1, tags: {type: flow1, category: category1}, conditions: [{operator: match_regex, parameters: {inputs: [{address: arg1}], regex: ^string.*}}, {operator: match_regex, parameters: {inputs: [{address: arg2}], regex: .*}}]}]})");
    ASSERT_TRUE(rule.type != DDWAF_OBJ_INVALID);

    ddwaf_handle handle = ddwaf_init(&rule, nullptr, nullptr);
    ASSERT_NE(handle, nullptr);
    ddwaf_object_free(&rule);

    ddwaf_context context = ddwaf_context_init(handle);
    ASSERT_NE(context, nullptr);

    ddwaf_object param1 = DDWAF_OBJECT_MAP, param2 = DDWAF_OBJECT_MAP, tmp;

    ddwaf_object_map_add(&param1, "arg1", ddwaf_object_string(&tmp, "not string 1"));
    ddwaf_object_map_add(&param1, "arg2", ddwaf_object_string(&tmp, "string 2"));
    ddwaf_object_map_add(&param2, "arg1", ddwaf_object_string(&tmp, "string 1"));

    // Run with both arg1 and arg2, but arg1 is wrong
    //	// Run with just arg1
    ddwaf_result ret;
    auto code = ddwaf_run(context, &param1, &ret, LONG_TIME);
    EXPECT_EQ(code, DDWAF_OK);
    EXPECT_FALSE(ret.timeout);
    ddwaf_result_free(&ret);

    // Override `arg1`
    code = ddwaf_run(context, &param2, &ret, LONG_TIME);
    EXPECT_EQ(code, DDWAF_MATCH);
    EXPECT_STREQ(ret.data,
        R"([{"rule":{"id":"1","name":"rule1","tags":{"type":"flow1","category":"category1"}},"rule_matches":[{"operator":"match_regex","operator_value":"^string.*","parameters":[{"address":"arg1","key_path":[],"value":"string 1","highlight":["string 1"]}]},{"operator":"match_regex","operator_value":".*","parameters":[{"address":"arg2","key_path":[],"value":"string 2","highlight":["string 2"]}]}]}])");
    ddwaf_result_free(&ret);

    // Run again without change
    code = ddwaf_run(context, ddwaf_object_map(&tmp), &ret, LONG_TIME);
    EXPECT_EQ(code, DDWAF_OK);
    EXPECT_FALSE(ret.timeout);
    ddwaf_result_free(&ret);

    ddwaf_context_destroy(context);
    ddwaf_destroy(handle);
}
