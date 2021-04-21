workspace(
    name = "elasticnodemiddleware",
)

load("//:github.bzl", "es_github_archive")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

es_github_archive(
    name = "EmbeddedSystemsBuildScripts",
    version = "1.0.2",
    # sha256 = "<checksum>"
)

load("@EmbeddedSystemsBuildScripts//Toolchains/Avr:avr.bzl", "avr_toolchain")

avr_toolchain()

http_archive(
    name = "Unity",
    build_file = "@EmbeddedSystemsBuildScripts//:BUILD.Unity",
    strip_prefix = "Unity-master",
    urls = ["https://github.com/ThrowTheSwitch/Unity/archive/master.tar.gz"],
)

http_archive(
    name = "CException",
    build_file = "@EmbeddedSystemsBuildScripts//:BUILD.CException",
    strip_prefix = "CException-master",
    urls = ["https://github.com/ThrowTheSwitch/CException/archive/master.tar.gz"],
)

http_archive(
    name = "CMock",
    build_file = "@EmbeddedSystemsBuildScripts//:BUILD.CMock",
    strip_prefix = "CMock-master",
    urls = ["https://github.com/ThrowTheSwitch/CMock/archive/master.tar.gz"],
)

http_archive(
    name = "LUFA",
    build_file = "@EmbeddedSystemsBuildScripts//:BUILD.LUFA",
    strip_prefix = "lufa-LUFA-170418",
    urls = ["https://fourwalledcubicle.com/files/LUFA/LUFA-170418.zip"],
)

es_github_archive(
    name = "EmbeddedUtilities",
    version = "0.3.2",
)

es_github_archive(
    name = "PeripheralInterface",
    version = "0.7.1"
)

# to use python requirements for packages
#http_archive(
#    name = "rules_python",
#    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
#    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
#)

#load("@rules_python//python:pip.bzl", "pip_install")

#pip_install(
#   name = "my_deps",
#   requirements = "//:requirements.txt",
#)
