load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "caching-proxy",
    srcs = ["main.cpp"],
    deps = [
        "@asio",
        "@cli11",
        "@curl",
        "@nlohmann_json//:json",
    ],
)
