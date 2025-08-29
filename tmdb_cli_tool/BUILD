load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "tmdb-app",
    srcs = ["main.cpp"],
    deps = [
        "@cli11",
        "@curl",
        "@nlohmann_json//:json",
    ],
)
