load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "utils",
    srcs = ["utils.cpp"],
    hdrs = ["utils.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@cpp-httplib//:httplib",
        "@fmt",
        "@nlohmann_json//:json",
    ],
)

cc_binary(
    name = "github-activity",
    srcs = ["main.cpp"],
    deps = [
        "@nlohmann_json//:json",
        # "@fmt//:fmt",
        "@fmt",
        ":utils",
    ],
)
