workspace(
    name = "elasticnodemiddleware",
)

load("//:github.bzl", "es_github_archive")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

es_github_archive(
    name = "EmbeddedSystemsBuildScripts",
    version = "0.6.1",
    # sha256 = "<checksu m>"
)

load("@EmbeddedSystemsBuildScripts//AvrToolchain:avr.bzl", "avr_toolchain")

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
    version = "0.3.1",
)

#es_github_archive(
#    name = "PeripheralInterface",
#    version = "0.6"
#)
local_repository(
    name = "PeripheralInterface",
    path = "../PeripheralInterface",
)