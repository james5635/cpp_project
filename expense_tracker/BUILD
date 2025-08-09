load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")


cc_library(
    name = "expense",
    srcs = ["expense.cpp"],
    hdrs = ["expense.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "expense_manager",
    srcs = ["expense_manager.cpp"],
    hdrs = ["expense_manager.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":expense",
        "@fmt",
        "@nlohmann_json//:json",
    ],
)

cc_binary(
    name = "expense-tracker",
    srcs = ["main.cpp"],
    deps = [
        "@nlohmann_json//:json",
        # "@fmt//:fmt",
        "@fmt",
        "@cli11",
        ":expense_manager",
    ],
)

