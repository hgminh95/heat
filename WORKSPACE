workspace(name = "heat")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)

git_repository(
    name = "libsmc",
    build_file = "//third_party:libsmc.BUILD",
    commit = "ba8e75a8b2ad70d195090f98a4e426d35bd16616",
    remote = "git@github.com:beltex/libsmc.git",
)

git_repository(
    name = "smctemp",
    build_file = "//third_party:smctemp.BUILD",
    commit = "8b416626095fc0983ce2421a24a92f1dcaf640ad",
    remote = "git@github.com:narugit/smctemp.git",
)
