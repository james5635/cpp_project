load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_binary(
    name = "github-activity",
    srcs = ["main.cpp"],
    deps = [
        "@cpp-httplib//:httplib",
        "@googletest//:gtest",
        "@nlohmann_json//:json",
        # "@fmt//:fmt",
        "@fmt",
    ],
)
