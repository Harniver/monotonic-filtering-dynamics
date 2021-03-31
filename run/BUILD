cc_library(
    name = "monotonic_filtering",
    hdrs = ["monotonic_filtering.hpp"],
    srcs = ['monotonic_filtering.cpp'],
    deps = [
        "@fcpp//lib:fcpp",
    ],
    visibility = [
        '//visibility:public',
    ],
)

cc_binary(
    name = "batch",
    srcs = ["batch.cpp"],
    deps = [
        "//run:monotonic_filtering",
    ],
)
