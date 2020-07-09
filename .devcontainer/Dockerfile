FROM ubuntu:18.04

# install bazel
RUN apt update
RUN apt install -y curl gnupg
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt update
# this is clearly version specific
RUN apt install -y bazel-2.2.0
RUN ln -s /usr/bin/bazel-2.2.0 /usr/bin/bazel
RUN bazel-2.2.0

#install others
RUN apt install -y git screen

# avr
RUN apt install -y avrdude avr-libc

# cleanup extras
RUN apt purge -y curl gnupg

VOLUME /app
WORKDIR /app