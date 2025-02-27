// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.

#include "test.h"

TEST(TestManifest, TestEmpty)
{
    auto manifest = ddwaf::manifest_builder().build_manifest();
    EXPECT_FALSE(manifest.contains("path"));
    EXPECT_TRUE(manifest.empty());

    auto [res, id] = manifest.get_target("path");
    EXPECT_FALSE(res);
    EXPECT_EQ(id, 0);
}

TEST(TestManifest, TestBasic)
{
    ddwaf::manifest_builder mb;
    auto target = mb.insert("path", {});

    {
        auto opt_target = mb.find("path");
        EXPECT_TRUE(opt_target.has_value());
        EXPECT_EQ(*opt_target, target);
    }

    // Test double insertion
    EXPECT_EQ(target, mb.insert("path", {}));

    auto manifest = mb.build_manifest();

    EXPECT_TRUE(manifest.contains("path"));
    EXPECT_FALSE(manifest.empty());

    auto [res, id] = manifest.get_target("path");
    auto info = manifest.get_target_info(id);
    EXPECT_TRUE(info.key_path.empty());
    EXPECT_STREQ(info.name.c_str(), "path");
    EXPECT_EQ(target, id);

    // This is it's own root address
    EXPECT_EQ(manifest::get_root(id), id);

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 1);
    EXPECT_STREQ(addresses[0], "path");
}

TEST(TestManifest, TestMultipleAddrs)
{
    ddwaf::manifest_builder mb;

    std::map<std::string, manifest::target_type> targets;
    for (const std::string str : {"path0", "path1", "path2", "path3"}) {
        auto target = mb.insert(str, {});
        targets[str] = target;

        // Test double insertion
        EXPECT_EQ(target, mb.insert(str, {}));

        auto opt_target = mb.find(str);
        EXPECT_TRUE(opt_target.has_value());
        EXPECT_EQ(*opt_target, target);
    }

    auto manifest = mb.build_manifest();

    for (const std::string str : {"path0", "path1", "path2", "path3"}) {
        EXPECT_TRUE(manifest.contains(str));

        auto [res, id] = manifest.get_target(str);
        auto info = manifest.get_target_info(id);
        EXPECT_TRUE(info.key_path.empty());
        EXPECT_EQ(targets[str], id);

        // This is it's own root address
        EXPECT_EQ(manifest::get_root(id), id);
    }

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 4);

    for (const std::string str : {"path0", "path1", "path2", "path3"}) {
        EXPECT_NE(find(addresses.begin(), addresses.end(), str), addresses.end());
    }
}

TEST(TestManifest, TestBasicKeyPath)
{
    ddwaf::manifest_builder mb;
    std::vector<std::string> key_path{"key_path"};

    auto target = mb.insert("path", key_path);

    // Test double insertion
    EXPECT_EQ(target, mb.insert("path", key_path));

    auto manifest = mb.build_manifest();

    EXPECT_TRUE(manifest.contains("path"));
    EXPECT_FALSE(manifest.empty());

    auto info = manifest.get_target_info(target);
    EXPECT_TRUE(info.key_path == key_path);
    EXPECT_STREQ(info.name.c_str(), "path");

    auto [res, root_id] = manifest.get_target("path");
    EXPECT_EQ(manifest::get_root(target), root_id);

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 1);
    EXPECT_STREQ(addresses[0], "path");
}

TEST(TestManifest, TestMultipleAddrsKeyPath)
{
    ddwaf::manifest_builder mb;

    std::vector<std::string> key_path{"key_path"};
    std::map<std::string, manifest::target_type> targets;
    for (auto str : {"path0", "path1", "path2", "path3"}) {
        auto target = mb.insert(str, key_path);
        targets[str] = target;

        // Test double insertion
        EXPECT_EQ(target, mb.insert(str, key_path));
    }

    auto manifest = mb.build_manifest();

    for (auto &[name, id] : targets) {
        auto [res, root_id] = manifest.get_target(name);
        auto info = manifest.get_target_info(id);
        EXPECT_TRUE(info.key_path == key_path);
        EXPECT_EQ(manifest::get_root(id), root_id);
        EXPECT_STREQ(info.name.c_str(), name.c_str());
    }

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 4);
    for (const std::string str : {"path0", "path1", "path2", "path3"}) {
        EXPECT_NE(find(addresses.begin(), addresses.end(), str), addresses.end());
    }
}

TEST(TestManifest, TestBasicMultiKeyPath)
{
    ddwaf::manifest_builder mb;
    std::vector<std::string> key_path{"first", "second", "last"};

    auto target = mb.insert("path", key_path);

    // Test double insertion
    EXPECT_EQ(target, mb.insert("path", key_path));

    auto manifest = mb.build_manifest();

    EXPECT_TRUE(manifest.contains("path"));
    EXPECT_FALSE(manifest.empty());

    auto info = manifest.get_target_info(target);
    EXPECT_TRUE(info.key_path == key_path);
    EXPECT_STREQ(info.name.c_str(), "path");

    auto [res, root_id] = manifest.get_target("path");
    EXPECT_EQ(manifest::get_root(target), root_id);

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 1);
    EXPECT_STREQ(addresses[0], "path");
}

TEST(TestManifest, TestMultipleAddrsMultiKeyPath)
{
    ddwaf::manifest_builder mb;

    std::vector<std::string> key_path{"first", "second", "last"};
    std::map<std::string, manifest::target_type> targets;
    for (auto str : {"path0", "path1", "path2", "path3"}) {
        auto target = mb.insert(str, key_path);
        targets[str] = target;

        // Test double insertion
        EXPECT_EQ(target, mb.insert(str, key_path));
    }

    auto manifest = mb.build_manifest();

    for (auto &[name, id] : targets) {
        auto [res, root_id] = manifest.get_target(name);
        auto info = manifest.get_target_info(id);
        EXPECT_TRUE(info.key_path == key_path);
        EXPECT_EQ(manifest::get_root(id), root_id);
        EXPECT_STREQ(info.name.c_str(), name.c_str());
    }

    auto &addresses = manifest.get_root_addresses();
    EXPECT_EQ(addresses.size(), 4);
    for (const std::string str : {"path0", "path1", "path2", "path3"}) {
        EXPECT_NE(find(addresses.begin(), addresses.end(), str), addresses.end());
    }
}

TEST(TestManifest, TestUnknownArgID)
{
    ddwaf::manifest manifest({}, {});
    EXPECT_FALSE(manifest.contains({}));
}
