cc_library(
    name = "strSplit_lib",
    srcs = ["strSplit.cpp"],
    hdrs = ["strSplit.h"],
)

cc_binary(
    name = "strSplit",
    srcs = ["strSplit_main.cpp"],
    deps = [":strSplit_lib"],
)

cc_test(
    name = "strSplit_test",
    srcs = ["strSplit_test.cpp"],
    deps = [
        ":strSplit_lib",
        "//:catch",
    ],
)
