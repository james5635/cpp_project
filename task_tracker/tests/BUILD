load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_test(
    name = "all_tests",
    size = "small",
    srcs = [
        "task.test.cpp",
        "task_manager.test.cpp",
    ],
    deps = [
        "//:task",
        "//:task_manager",
        "@catch2//:catch2_main",
        "@nlohmann_json//:json",
    ],
)
