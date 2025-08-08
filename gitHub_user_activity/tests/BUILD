load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "all_tests",
    srcs = ["utils_tests.cpp"],
    size = "small",

    deps = [
        "//:utils",
        "@nlohmann_json//:json",
        "@googletest//:gtest_main"
    ]
)
