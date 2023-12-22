cc_library(
    name = "smctemp",
    hdrs = glob([
        "*.h"
    ]),
    srcs = glob([
        "*.cc",
    ], exclude = [
        "main.cc"
    ]),
    includes = ["."],
    defines = [
        "ARCH_TYPE_ARM64",
    ],
    visibility = [
        "//visibility:public",
    ]
)
