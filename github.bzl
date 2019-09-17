load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

URL = "https://github.com/es-ude/{repo}/archive/v{version}.tar.gz"

def es_github_archive(name, version, repo = "",  **kwargs):
  '''Convenient wrapper for embedded systems department
     open source projects. It assumes version tags
     are prefixed with the letter 'v'.'''
  if repo == "":
      repo = name
  http_archive(
    name = name,
    strip_prefix = "{}-{}".format(name, version),
    urls = [URL.format(repo = repo, version = version)],
    **kwargs
  )
