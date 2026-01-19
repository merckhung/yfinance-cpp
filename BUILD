cc_library(
    name = "yfinance",
    srcs = glob(["cpp/*.cpp"]),
    hdrs = glob(["hpp/*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        "@cpr//:cpr",
        "@curl//:curl",
        "@nlohmann_json//:json",
    ],
)

cc_binary(
    name = "news",
    srcs = ["demo/news.cpp"],
    deps = [":yfinance"],
)

cc_binary(
    name = "options",
    srcs = ["demo/options.cpp"],
    deps = [":yfinance"],
)

cc_binary(
    name = "quotes",
    srcs = ["demo/quotes.cpp"],
    deps = [":yfinance"],
)

cc_binary(
    name = "summary",
    srcs = ["demo/summary.cpp"],
    deps = [":yfinance"],
)

cc_binary(
    name = "tsla_1m",
    srcs = ["demo/tsla_1m.cpp"],
    deps = [":yfinance"],
)

cc_binary(
    name = "tsla_options_3w",
    srcs = ["demo/tsla_options_3w.cpp"],
    deps = [":yfinance"],
)
