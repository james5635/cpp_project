load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")


cc_library(
    name = "task",
    srcs = ["task.cpp"],
    hdrs = ["task.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "task_manager",
    srcs = ["task_manager.cpp"],
    hdrs = ["task_manager.h"],
    visibility = ["//visibility:public"],
    deps = [":task"],
)

cc_binary(
    name = "task-cli",
    srcs = ["main.cpp"],
    deps = [
        ":task_manager",
        "@fmt",
    ],
)
